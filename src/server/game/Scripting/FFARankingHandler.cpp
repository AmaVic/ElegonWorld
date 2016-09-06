

#include "FFARankingHandler.h"
#include "Player.h"
#include "WorldSession.h"
#include <algorithm>


void FFARankingHandler::registerPvPKill(uint64 killerGUID, uint64 killedGUID)
{
	//On regarde si le personnage tueur est déjà enregistré dans la table
	QueryResult res = CharacterDatabase.PQuery("SELECT * FROM ffarankings where playerGUID=%u", killerGUID);
	Field* answer = res->Fetch();
	if (!res)
	{
		CharacterDatabase.PExecute("INSERT INTO ffarankings VALUES(%u, 0, 0, 0)", killerGUID);

	}

	//On en fait de même pour le tué
	QueryResult killedResult = CharacterDatabase.PQuery("SELECT count(*) FROM ffarankings where playerGUID=%u", killedGUID);
	Field* killerAnsw = killedResult->Fetch();
	if (killerAnsw[0].GetUInt32() == 0)
	{
		CharacterDatabase.PExecute("INSERT INTO ffarankings VALUES(%u, 0, 0, 0)", killedGUID);
	}

	//Ensuite, on modifie les deux lignes pour ajouter le kill à chacun (comme un kill et comme un décès respectivement au tueur et au tué
	CharacterDatabase.PExecute("UPDATE ffarankings SET FFAKills=FFAKills+1 WHERE playerGUID=%u", killerGUID);
	CharacterDatabase.PExecute("UPDATE ffarankings SET FFADeaths=FFADeaths+1 WHERE playerGUID=%u", killedGUID);

	//Mise à jour de la plus grosse série d'élimination du joueur:

	//1: récup de la plus grosse série
	QueryResult KSResult = CharacterDatabase.PQuery("SELECT FFAMaxKS FROM ffarankings WHERE playerGUID=%u", killerGUID);
	if (!KSResult)
	{
		return;
	}
	Field* KSFields = KSResult->Fetch();
	uint32 dbBest = KSFields[0].GetUInt32();
	Player* p = sObjectAccessor->FindPlayer(killerGUID);
	if (!p)
		return;
	uint32 currKS = p->GetKillStreak();
	
	//On regarde si le record a été battu ou non
	uint32 best = std::max(dbBest, currKS);
	//On met à jour les données dans la DB
	CharacterDatabase.PExecute("UPDATE ffarankings SET FFAMaxKS=%u WHERE playerGUID=%u", best, killerGUID);
}


FFARankingHandler::Scores FFARankingHandler::generateRanking(FFARankingHandler::OrderCriteria order)
{

	Scores scores;

	//On récupère les données dans la DB
	QueryResult result;
	switch (order)
	{
	case BY_KILLS:
		result = CharacterDatabase.PQuery("SELECT * FROM ffarankings ORDER BY FFAKills DESC");
		break;
	case BY_RATIO:
		result = CharacterDatabase.PQuery("SELECT * FROM ffarankings ORDER BY FFAKills/FFADeaths DESC");
		break;
	case BY_MAX_KILLSTREAK:
		result = CharacterDatabase.PQuery("SELECT * FROM ffarankings ORDER BY FFAMaxKS DESC");
		break;
	}

	if (!result)
	{
		return scores;
	}

	do{
		Field* fields = result->Fetch();
		FFAScore currScore;
		currScore.guid = fields[0].GetUInt64();
		currScore.kills = fields[1].GetUInt32();
		currScore.deaths = fields[2].GetUInt32();
		currScore.ratio = (currScore.deaths == 0) ? (double)currScore.kills : (double)((double)currScore.kills / (double)currScore.deaths);
		currScore.maxKS = fields[3].GetUInt32();

		scores.push_back(currScore);

	} while (result->NextRow());


	return scores;
}

FFARankingHandler::OrderCriteria FFARankingHandler::strToOrderCriteria(const char* str)
{
	if (str == std::string("kills").c_str())
		return BY_KILLS;
	if (str == std::string("ratio").c_str())
		return BY_RATIO;
	if (str == std::string("serie").c_str())
		return BY_MAX_KILLSTREAK;
}