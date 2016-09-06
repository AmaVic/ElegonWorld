
#include "ScriptMgr.h"
#include "Vehicle.h"
#include "FFARankingHandler.h"
#include "MastoHandler.h"
#include "GoldScore.h"
#include "VHScore.h"

#include <sstream>
#include <memory>

#define AURA_FLAG_ALLIANCE 23335
#define AURA_FLAG_HORDE    23333

#define GO_BUFF_ITEM 655656

#define FOOD_BUFF 23493
#define SPELL_SLOW_DOWN 23978
#define SPELL_WINGS 31884
#define SPELL_SPEED 23978

#define SPELL_BOOM_VISUAL 58538
#define SPELL_BOOM_DMG 63482

#define SPELL_PARACHUTE 55001

#define HONOR_POINTS 1000
#define ARENA_POINTS 5

#define ID_TRANS 33062

//Zone shop
#define ZS_MAP_ID 0
#define ZS_X 0
#define ZS_Y 0
#define ZS_Z 0
#define ZS_O 0

#define GOSSIP_RANKING_KILLS "Classement par victimes"
#define GOSSIP_RANKING_RATIO "Classement par ratio"
#define GOSSIP_RANKING_KS "Classement par killstreak"
#define GOSSIP_BYE "Au revoir"


class script_ks : public PlayerScript
{

	//Script KillStreak: ajout des KS et récompense / evt


public: //Constructeur

	script_ks() : PlayerScript("script_ks")
	{

	}


public: //Override

	void OnPVPKill(Player* killer, Player* killed)
	{


		if (killer != killed)
			killer->KilledMonsterCredit(3008);

		if (sMastoHandler->isMasto(killed->GetGUID()))
			sMastoHandler->onMastoKilled();

		if (killed->GetAreaId() != 720)
			return;
		if (killer == killed)
			return;

		killer->AddAura(FOOD_BUFF, killer);

		//La série du tueur augmente de 1, celle du tué est réinitialisée (0)
		killer->SetKillStreak(killer->GetKillStreak() + 1);
		killed->SetKillStreak(0);

		//Le tueur reçoit la "récompense" liée à sa série d'élimination
		//Le tué perds sa série et les bonus liés
		RewardPlayerForKS(killer);
		RewardPlayerForKS(killed);

		killer->SetHonorPoints((HONOR_POINTS * killer->GetKillStreak() ) + killer->GetHonorPoints());
		killer->SetArenaPoints(killer->GetArenaPoints() +( ARENA_POINTS*killer->GetKillStreak()));


		//Boule:
		GameObject* go = killer->SummonGameObject(GO_BUFF_ITEM, killed->GetPositionX(), killed->GetPositionY(), killed->GetPositionZ(), 0.f, 0.f, 0.f, 0.f, 0.f, 0);

		if (killer->GetAreaId() == 720)
		{
			//killed->ResurrectPlayer(100.f);
			//killed->TeleportTo(ZS_MAP_ID, ZS_X, ZS_Y, ZS_Z, ZS_O);
		}

		FFARankingHandler::registerPvPKill(killer->GetGUID(), killed->GetGUID());

	}

	void OnPlayerKilledByCreature(Creature* killer, Player* killed)
	{
		killed->SetKillStreak(0);
		RewardPlayerForKS(killed);
	}

public: //Méthodes publiques

	static void RewardPlayerForKS(Player* player)
	{
		//Quelque soit le KS: on augmente la taille du joueur légèrement:

		if (player->GetKillStreak() <= 5){
			player->SetObjectScale(1.f + player->GetKillStreak()*0.1);
		}


		//Si KS >= 3: le joueur se recoit une aura
		if (player->GetKillStreak() >= 2)
		{
			player->KilledMonsterCredit(3007);
			//Alliance 
			if (player->GetTeamId() == TEAM_ALLIANCE)
			{
				if (!player->HasAura(AURA_FLAG_ALLIANCE))
					player->AddAura(AURA_FLAG_ALLIANCE, player);
			}
			//Horde
			else if (player->GetTeamId() == TEAM_HORDE)
			{
				if (!player->HasAura(AURA_FLAG_HORDE))
					player->AddAura(AURA_FLAG_HORDE, player);
			}

		}

		if (player->GetKillStreak() >= 3)
		{
			player->KilledMonsterCredit(3007);

			player->CastSpell(player, SPELL_BOOM_DMG);

			/*
			Creature* go = player->SummonCreature(33062, player->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 360000);
			if (!go->IsVehicle())
				return;
			Vehicle* v = go->GetVehicleKit();
			if (!v)
				return;

			player->EnterVehicle(v->GetBase(), 0);
			*/
		}

	}

};

class script_moveZone_ks : public PlayerScript
{

public: //Constructeur

	script_moveZone_ks() : PlayerScript("script_moveZone_ks"){}

public: //Override

	void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/)
	{
		player->SetKillStreak(0);
		script_ks::RewardPlayerForKS(player);
	}

};


class script_boule_ks : public GameObjectScript
{

public: //Constructeur

	script_boule_ks() : GameObjectScript("script_boule_ks") {

	}


private: //méthodes privées:

	uint32 choseSpell()
	{
		int r = rand() % 101;
		
		if (r >= 75)
			return SPELL_WINGS;
		if (r >= 50 && r <= 60)
			return SPELL_SLOW_DOWN;
		if (r <= 40)
			return SPELL_SPEED;

		return SPELL_SPEED;

	}

public: //Override:

	bool OnGossipHello(Player* player, GameObject* go)
	{
		player->CLOSE_GOSSIP_MENU();
		go->RemoveFromWorld();

		player->AddAura(choseSpell(), player);

		return false;
	}

};

class script_ks_init_reset : public PlayerScript
{

public: //Constructeur

	script_ks_init_reset() : PlayerScript("script_ks_init") {}

public: //Overrie:

		void OnLogin(Player* player, bool firstLogin)
	{
		//Reset KS & taille normale
		player->SetKillStreak(0);
		player->SetObjectScale(1.f);

	}

	void OnLogout(Player* player)
	{
		//Reset KS & taille normale
		player->SetKillStreak(0);
		player->SetObjectScale(1.f);
	}

};

class script_ks_commands : public CommandScript
{

public:
	script_ks_commands() : CommandScript("script_ks_commands"){}

	ChatCommand* GetCommands() const override
	{

		static ChatCommand commandTable[] =
		{
			{ "ffa", rbac::RBAC_PERM_COMMAND_FFA_RANKING, false, &HandleFFARankingCommand, "", NULL },
			{ NULL, 0, false, NULL }
		};

		return commandTable;

	}

	static bool HandleFFARankingCommand(ChatHandler* handler, char const* args)
	{

		if (!*args)
			return false;

		char* criteriaStr = strtok((char*)args, " ");
		if (!criteriaStr)
			return false;

		int crit = atoi(criteriaStr);
		if (crit != 0 && crit != 1 && crit != 2)
			crit = 0;
		FFARankingHandler::OrderCriteria criteria = static_cast<FFARankingHandler::OrderCriteria>(crit);

		FFARankingHandler::Scores scores = FFARankingHandler::generateRanking(criteria);

		return true;
	}

};

class script_npc_classement : public CreatureScript
{

public:

	script_npc_classement() : CreatureScript("script_npc_classement"){}

public:

	virtual bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(0, GOSSIP_RANKING_KILLS, GOSSIP_SENDER_MAIN, 0);
		player->ADD_GOSSIP_ITEM(0, GOSSIP_RANKING_RATIO, GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, GOSSIP_RANKING_KS, GOSSIP_SENDER_MAIN, 2);

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;
	}

	virtual bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{

		if (action >= 100)
		{
			player->PlayerTalkClass->ClearMenus();
			player->CLOSE_GOSSIP_MENU();
			return true;
		}
		player->PlayerTalkClass->ClearMenus();

		FFARankingHandler::Scores scores = FFARankingHandler::generateRanking(static_cast<FFARankingHandler::OrderCriteria>(action));
		for(std::size_t i = 0; i < scores.size(); i++)
		{
			FFARankingHandler::FFAScore currScore = scores.at(i);

			std::string text;
			//Nom du joueur
			Player* p = sObjectAccessor->FindPlayer(currScore.guid);
			if (!p)
			{
				continue;
			}
			//Rang:
			text += std::to_string(i + 1);
			//playerName:
			text += ": "; 
			
			QueryResult res = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid=%u", currScore.guid);
			if (!res)
			{
				continue;
			}
			Field* f = res->Fetch();
			text += f[0].GetString();

			text += ": ";
			//carac:
			switch (action)
			{
			case 0:
				text += std::to_string(currScore.kills); text += " victimes";
				break;
			case 1:
				text += std::to_string(currScore.ratio); text += " (ratio)";
				break;
			case 2:
				text += std::to_string(currScore.maxKS); text += " (plus grande serie)";
				break;
			}

			player->ADD_GOSSIP_ITEM(0, text.c_str(), GOSSIP_SENDER_MAIN, 100+i);

		}

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());


		return true;
	}


};





//Ajout du script
void AddSC_script_ks()
{
	new script_ks();
	new script_ks_init_reset();
	new script_moveZone_ks();
	new script_boule_ks();
	new script_ks_commands();
	new script_npc_classement();
}

