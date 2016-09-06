
#include "SponsorshipHandler.h"
#include "WorldSession.h"


//-------------------------------------------
//Méthodes publiques
//-------------------------------------------
bool SponsorshipHandler::sponsoringPossible(uint32 sponsorAccountId, uint32 sponsoredAccountId)
{
	//Si qqn essaye de se parrainer lui-même, on l'interdit
	if (sponsorAccountId == sponsoredAccountId)
		return false;

	//On regarde si l'IP des deux comptes est différente
	//Ip du parrain:
	QueryResult sponsorIPResult = LoginDatabase.PQuery("SELECT last_ip FROM account where id=%u", sponsorAccountId);
	Field* sponsorIPFields = sponsorIPResult->Fetch();
	std::string sponsorIP = sponsorIPFields[0].GetString();

	//Ip du parrainé:
	QueryResult sponsoredIPResult = LoginDatabase.PQuery("SELECT last_ip FROM account where id=%u", sponsoredAccountId);
	Field* sponsoredIPFields = sponsoredIPResult->Fetch();
	std::string sponsoredIP = sponsoredIPFields[0].GetString();

	//On regarde si les IP sont différentes. Si ce n'est pas le cas, le parrainage n'est pas autorisé
	if (sponsoredIP == sponsorIP)
		return false;

	//On récupère le temps de jeu cumulé (= sur tous les joueurs) du parrain:
	QueryResult sponsorTimeResult = CharacterDatabase.PQuery("SELECT totaltime FROM characters WHERE account = %u", sponsorAccountId);
	uint32 sponsorPlayedTime = 0;
	do
	{
		Field* sponsorTimeFields = sponsorTimeResult->Fetch();
		sponsorPlayedTime += sponsorTimeFields[0].GetUInt32();
	} while (sponsorTimeResult->NextRow());

	//Si le parrain a jouté moins de 24h en tout sur le serveur, il ne peut pas parrainé
	if (sponsorPlayedTime < ONE_DAY_IN_SEC)
		return false;

	//Si on arrive ici, c'est que les IP des deux personnes sont différentes et que la parrain a joué plus de 24h
	//Le parrainage est donc accepté
	return true;
}


bool SponsorshipHandler::playerExists(const std::string& playerName)
{
	Player* player = sObjectAccessor->FindPlayerByName(playerName);
	if (!player)
		return false;

	if (player && !player->IsInWorld())
		return false;

	return true;
}

uint32 SponsorshipHandler::findAccountId(const std::string& playerName) 
{
	Player* player = sObjectAccessor->FindPlayerByName(playerName);
	if (!player)
		return 0;

	return player->GetSession()->GetAccountId();
}

bool SponsorshipHandler::registerSponsorship(uint32 sponsorAccountId, uint32 sponsoredAccountId)
{
	QueryResult checkResult = LoginDatabase.PQuery("SELECT count(*) FROM sponsorship WHERE sponsorAccountId=%u AND sponsoredAccountId=%u", sponsorAccountId, sponsoredAccountId);
	Field* checkFields = checkResult->Fetch();
	uint32 count = checkFields[0].GetUInt32();

	if (count != 0)
		return false;

	LoginDatabase.PQuery("INSERT INTO sponsorship VALUES(%u, %u, CURRENT_DATE, CURRENT_DATE + INTERVAL 30  DAY)", sponsorAccountId, sponsoredAccountId);

	return true;
}

bool SponsorshipHandler::areInSponsorship(uint32 firstAccountId, uint32 secondAccountId)
{
	QueryResult checkResult = LoginDatabase.PQuery("SELECT count(*) FROM sponsorship WHERE (sponsorAccountId=%u AND sponsoredAccountId=%u) OR (sponsorAccountId=%u AND sponsoredAccountId=%u)", firstAccountId, secondAccountId, secondAccountId, firstAccountId);
	Field* checkFields = checkResult->Fetch();
	uint32 count = checkFields[0].GetUInt32();

	return count > 0;
}


void SponsorshipHandler::removeSponsorshipIfExpired()
{
	LoginDatabase.PExecute("DELETE FROM sponsorship WHERE endDate <= CURRENT_DATE");
}

bool SponsorshipHandler::findOnlineSponsorOrSponsored(uint32 accId, uint64 commandUserGUID, uint64& guid)
{
	QueryResult searchResult = LoginDatabase.PQuery("SELECT guid, count(*) FROM characters.characters WHERE ( (account IN(SELECT sponsorAccountId FROM auth.sponsorship) OR account IN(SELECT sponsoredAccountId FROM auth.sponsorship)) AND account <> %u) AND online = 1", accId, commandUserGUID);
	Field* fields = searchResult->Fetch();
	
	//si le parrain/parrainé n'est pas en ligne: on stop
	uint32 count = fields[1].GetUInt32();
	if (count == 0)
	{
		guid = 0;
		return false;
	}

	//Sinon, met le guid en place et on retourne true
	guid = fields[0].GetUInt64();
	return true;
}

bool SponsorshipHandler::isInSponsorship(uint32 accId)
{	
	QueryResult res = LoginDatabase.PQuery("SELECT count(*) FROM sponsorship WHERE endDate > CURRENT_DATE AND (sponsoredAccountId=%u OR sponsorAccountId=%u)", accId, accId);
	Field* fields = res->Fetch();

	if (!fields)
		return false;

	return fields[0].GetUInt32() > 0;
}

uint32 SponsorshipHandler::timeLeft(uint32 accId)
{
	QueryResult res = LoginDatabase.PQuery("SELECT TO_DAYS(endDate) - TO_DAYS(startDate) FROM sponsorship WHERE sponsorAccountId=%u OR sponsoredAccountId=%u", accId, accId);
	Field* f = res->Fetch();

	return f[0].GetUInt32();
}