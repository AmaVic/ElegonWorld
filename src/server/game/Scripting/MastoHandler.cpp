
#include "MastoHandler.h"

#include "GameEventMgr.h"
#include "ObjectAccessor.h"
#include "World.h"
#include "Player.h"
#include "WorldSession.h"
#include "MapManager.h"
#include "GroupMgr.h"



//----------------------------------------
//Constantes
//----------------------------------------
MastoHandler::MastoHandler() 
: mTimer(0),
  mEventStarted(false),
  mMastodonte(nullptr),
  mGroup(nullptr),
  mMastoBaseHP(0),
  mMsg5minSent(false),
  mPlayersCountAtStart(0),
  mWinner(WINNER_NONE)
{
	
}

//----------------------------------------
//Constantes
//----------------------------------------

//ID de l'event:
//const uint32 MastoHandler::EVENT_MASTODONTE_ID = 120;

//Positions:												  mapId       x                 y          z          o
const WorldLocation MastoHandler::POS_MASTO = WorldLocation(MAP_ID, -1648.117676f, -4285.779297f, 19.065235f, 3.956810f);
const WorldLocation MastoHandler::POS_GROUP = WorldLocation(MAP_ID, -1671.438599f, -4317.183594f, 3.359515f, 0.933025f);

//Taille
const float MastoHandler::MASTO_SCALE = 3.f;

//HP
const uint32 MastoHandler::MASTO_BONUS_HP = 1000000;

//Timers
const uint32 MastoHandler::EVENT_DURATION    = 2 * MINUTE * IN_MILLISECONDS; //(millisec)
const uint32 MastoHandler::EVENT_REPEAT_TIME = 15 * MINUTE * IN_MILLISECONDS; //(millisec)

//Nombre de joueurs requires
const uint32 MastoHandler::EVENT_MIN_PLAYERS_COUNT = 3; //Au moins un masto et 2 joueurs pour l'affronter

//Messages
const uint32 MastoHandler::SAY_5MIN_BEFORE_EVENT  = 200000;
const uint32 MastoHandler::SAY_NOT_ENOUGH_PLAYERS = 200001;
const uint32 MastoHandler::SAY_NEXT_EVENT		  = 200002;
const uint32 MastoHandler::SAY_WINNER			  = 200003;

//Honneur que les joueurs recoivent en récompense
const uint32 MastoHandler::HONOR_REWARD_WINNER = 10000;
const uint32 MastoHandler::HONOR_REWARD_LOSER  = 2000;

//ID de la zone
const uint32 MastoHandler::AREA_ID = 720; //ID de la zone dans laquelle se déroule l'event
const uint32 MastoHandler::MAP_ID  = 1;
const uint32 MastoHandler::ZONE_ID = 0;

//ID de l'aura à appliquer au masto (pour montrer que c'est lui)
const uint32 MastoHandler::MASTO_AURA_ID = 1130;
const uint32 MastoHandler::AURA_DAMAGE_ID = 39089;


//----------------------------------------
//Fonctions membres privées
//----------------------------------------
float MastoHandler::timeBeforeEventStart() const
{
	return 0.f; //TODO
}

bool MastoHandler::eventCanBeStarted() const
{

	const Map::PlayerList& playersInMap = sMapMgr->FindMap(MAP_ID, 0)->GetPlayers();
	uint32 count = 0;


	for (auto it = playersInMap.begin(); it != playersInMap.end(); it = it->next())
	{
		//Décomenter en release
		if (/*!it->GetSource()->IsGameMaster() && */it->GetSource()->GetAreaId() == AREA_ID && it->GetSource()->IsInWorld())
			count++;
	}

	return count >= EVENT_MIN_PLAYERS_COUNT;
	return false;
}

void MastoHandler::choseMastodonte()
{
	const Map::PlayerList& playersInMap = sMapMgr->FindMap(MAP_ID, 0)->GetPlayers();
	auto it = playersInMap.begin();
	
	//On cherche un joueur éligible (NON-GM et dans la bonne zone)
	do
	{
		//On initialise l'itérateur
		it = playersInMap.begin();
		//On choisir un élément au hasard
		int i = 0;
		int rand = urand(0, playersInMap.getSize() - 1);
		while (i != rand)
		{
			it = it->next();
			i++;
		}
	} while ((it->GetSource()->GetAreaId() != AREA_ID) || !it->GetSource()->IsInWorld()/* || it->GetSource()->IsGameMaster()*/);
	//Décomenter pour la release
	
	mMastodonte = it->GetSource();
}

void MastoHandler::prepareMastodonte()
{

	//Taille
	mMastodonte->SetObjectScale(MASTO_SCALE);

	uint32 plcount = 0;
	const Map::PlayerList& playersInMap = sMapMgr->FindMap(MAP_ID, 0)->GetPlayers();
	for (auto it = playersInMap.begin(); it != playersInMap.end(); it = it->next())
	{
		//A décommenter pour la release
		if (/*!it->GetSource()->IsGameMaster() &&*/ it->GetSource()->GetAreaId() == AREA_ID && it->GetSource()->IsInWorld())
			plcount++;
	}

	mPlayersCountAtStart = plcount;

	if (mMastodonte->GetGroup())
		mMastodonte->RemoveFromGroup();

	if (mMastodonte->isDead())
		mMastodonte->ResurrectPlayer(100.f);

	//PvP: on
	mMastodonte->SetPvP(true);

	//Modification des stats:
	for (std::size_t i = 0; i < plcount-1; i++)
		mMastodonte->AddAura(AURA_DAMAGE_ID, mMastodonte);


	//Modif de la vie
	mMastoBaseHP = mMastodonte->GetMaxHealth();
	mMastodonte->SetMaxHealth(mMastodonte->GetMaxHealth() + MASTO_BONUS_HP * (plcount-1));
	mMastodonte->SetHealth(mMastodonte->GetMaxHealth());

	//Reset CD
	mMastodonte->RemoveAllSpellCooldown();

	//Ajout de l'aura:
	mMastodonte->AddAura(MASTO_AURA_ID, mMastodonte);


	//Téléportation
	mMastodonte->TeleportTo(POS_MASTO);
	mMastodonte->GetSession()->SendAreaTriggerMessage("Vous etes le mastodonte ! Decimez les tous !");

}

void MastoHandler::prepareGroup()
{
	mGroup = new Group();
	mGroup->ConvertToRaid();
	const Map::PlayerList& pl = sMapMgr->FindMap(MAP_ID, 0)->GetPlayers();
	for(auto it = pl.begin(); it != pl.end(); it = it->next())
	{
		if (!it->GetSource())
			continue;
		if (!it->GetSource()->IsInWorld())
			continue;
		if (mMastodonte)
			if (it->GetSource() == mMastodonte)
				continue;
		if (it->GetSource()->GetAreaId() != AREA_ID)
			continue;
		if (it->GetSource()->isDead())
			it->GetSource()->ResurrectPlayer(100.f);

		/*if (!mGroup->IsCreated())
		{
			mGroup->Create(it->GetSource());
			sGroupMgr->AddGroup(mGroup);
		}*/


		if(it->GetSource()->GetGroup() != mGroup)
			it->GetSource()->RemoveFromGroup();

		it->GetSource()->GetSession()->SendAreaTriggerMessage("Vous etes dans le groupe pour l'evenement Mastodonte");
		it->GetSource()->SetPvP(true);
		it->GetSource()->RemoveAllSpellCooldown();
		it->GetSource()->TeleportTo(POS_GROUP);
		if (mGroup->GetMembersCount() < 40)
		if (it->GetSource()->GetGroup() != mGroup)
			mGroup->AddMember(it->GetSource());
	}
}

void MastoHandler::sendEventStartedMessage()
{
	sWorld->SendWorldText(SAY_5MIN_BEFORE_EVENT);
}

void MastoHandler::rewardPlayers()
{
	if (mWinner == WINNER_MASTO)
	{
		if (mMastodonte)
			mMastodonte->SetHonorPoints(mMastodonte->GetHonorPoints() + HONOR_REWARD_WINNER);

		if (mGroup)
		{
			for (auto it = mGroup->GetMemberSlots().begin(); it != mGroup->GetMemberSlots().end(); it++)
			{
				Player* p = sObjectAccessor->FindPlayer(it->guid);
				if (!p)
					continue;
				p->SetHonorPoints(p->GetHonorPoints() + HONOR_REWARD_LOSER);
			}
		}
	}
	else if (mWinner == WINNER_GROUP)
	{
		if (mMastodonte)
			mMastodonte->SetHonorPoints(mMastodonte->GetHonorPoints() + HONOR_REWARD_LOSER);


		if (mGroup)
		{
			for (auto it = mGroup->GetMemberSlots().begin(); it != mGroup->GetMemberSlots().end(); it++)
			{
				Player* p = sObjectAccessor->FindPlayer(it->guid);
				if (!p)
					continue;
				p->SetHonorPoints(p->GetHonorPoints() + HONOR_REWARD_WINNER);
			}
		}
	}
}

void MastoHandler::resetMasto(Player* masto)
{

	if (!masto)
		return;

	if (masto->isDead())
	{
		masto->ResurrectPlayer(100.f);
	}

	masto->RemoveAura(AURA_DAMAGE_ID);
	masto->RemoveAura(MASTO_AURA_ID);

	masto->SetMaxHealth(mMastoBaseHP);
	masto->SetHealth(mMastoBaseHP);


	masto->SetObjectScale(1.f);

}

void MastoHandler::onEventEnd()
{
	if (mWinner == WINNER_MASTO)
		sWorld->SendWorldText(SAY_WINNER, "Le mastodonte");
	else
		sWorld->SendWorldText(SAY_WINNER, "La resistance");

	resetMasto(mMastodonte);
	rewardPlayers();
	if (mGroup)
	{
		if (mGroup->GetMembersCount() > 0)
			mGroup->Disband();
	}
	mEventStarted = 0;
	planNextEvent();
	mMsg5minSent = false;
	mMastodonte = nullptr;
}

void MastoHandler::planNextEvent()
{
	mTimer = 0;
	sWorld->SendWorldText(SAY_NEXT_EVENT);
}

//----------------------------------------
//Fonctions membres publiques
//----------------------------------------
void MastoHandler::update(uint32 diff)
{
	mTimer += diff;

	//Si l'event n'est pas encore lancé, on regarde si il est temps ou non de le lancer
	if (!mEventStarted)
	{
		//S'il reste 5m avant l'event, on envoie un msg
		if (mTimer < EVENT_REPEAT_TIME && (EVENT_REPEAT_TIME - mTimer <= 30000) && !mMsg5minSent)
		{
			sWorld->SendWorldText(SAY_5MIN_BEFORE_EVENT);
			mMsg5minSent = true;
		}
		
		//S'il n'est pas temps de lancer l'event, on ne fait rien, on stop ici
		if (mTimer < EVENT_REPEAT_TIME)
			return;
		//Si par contre il est temps de lancer l'event, on regarde si on peut lancer l'event et si oui, on le fait et on reset le timer pour l'utiliser pour calculer la durée de l'event
		if (!eventCanBeStarted())
		{
			sWorld->SendWorldText(SAY_NOT_ENOUGH_PLAYERS);
			sWorld->SendWorldText(SAY_NEXT_EVENT);
			mTimer = 0;

			return;
		}

		//On choisit un mastodonte, on prépare le groupe et c'est parti !
		mEventStarted = true;
		choseMastodonte();
		prepareMastodonte();
		prepareGroup();
		sendEventStartedMessage();
		mTimer = 0;
	}

	//Si on arrive ici, l'event a été lancé


	//Si on a dépassé le temps accordé pour tuer le mastodonte (il n'a pas été tué donc)
	if (mTimer >= EVENT_DURATION)
	{
		//Gagnant = Mastodonte
		mWinner = WINNER_MASTO;
		onEventEnd();
	}
}


bool MastoHandler::eventIsStarted() const
{
	return mEventStarted;
}

bool MastoHandler::isMasto(uint64 playerGUID) const
{
	if (!mEventStarted)
		return false;
	if (!mMastodonte)
		return false;

	return playerGUID == mMastodonte->GetGUID();
}

void MastoHandler::choseNewMastoAndKeepEventGoing()
{
	mMastodonte = nullptr;
	uint32 plcount = 0;
	const Map::PlayerList& playersInMap = sMapMgr->FindMap(MAP_ID, 0)->GetPlayers();
	for (auto it = playersInMap.begin(); it != playersInMap.end(); it = it->next())
	{
		//A décommenter pour la release
		if (/*!it->GetSource()->IsGameMaster() &&*/ it->GetSource()->GetAreaId() == AREA_ID && it->GetSource()->IsInWorld())
			plcount++;
	}

	if (plcount < EVENT_MIN_PLAYERS_COUNT)
	{
		mWinner = WINNER_GROUP;
		onEventEnd();
		return;
	}

	choseMastodonte();
	prepareMastodonte();
}

void MastoHandler::onMastoKilled()
{
	mWinner = WINNER_GROUP;
	onEventEnd();
}

