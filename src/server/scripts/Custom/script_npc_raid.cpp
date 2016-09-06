#include "ScriptMgr.h"
#include "RGM.h"

//NPC entry = 1000005
//Gossip entry = 10000
//Text entry = 50000


#define SAY_INFO "Qu'est-ce que la recherche de raid ?"
#define SAY_SUBSCRIBE "J'aimerais m'inscrire dans la recherche de raid"

#define SAY_ROLE_TANK "Tank"
#define SAY_ROLE_HEAL "Heal"
#define SAY_ROLE_DPS  "DPS"

#define MESSAGE_INFO_RAID_ID 0 //à définir

#define RAID_ID 0
#define RAID_DIFFICULTY 0

enum GoAction
{
	GOSSIP_ACTION_INFO = 0,
	GOSSIP_ACTION_CHOSE_ROLE = 1,
	GOSSIP_ACTION_SUBSCRIBE_TANK = 2,
	GOSSIP_ACTION_SUBSCRIBE_HEAL = 3,
	GOSSIP_ACTION_SUBSCRIBE_DPS = 4
};

class script_npc_raid : public CreatureScript
{

public: //Constructeur

	script_npc_raid() : CreatureScript("script_npc_raid") {}

public: //Override

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		player->ADD_GOSSIP_ITEM(0, SAY_INFO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO);
		player->ADD_GOSSIP_ITEM(0, SAY_SUBSCRIBE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHOSE_ROLE);

		player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
	{
		player->PlayerTalkClass->ClearMenus();
		bool canTank = false;
		bool canHeal = false;

		switch (action)
		{
		case GOSSIP_ACTION_INFO:
			player->SEND_GOSSIP_MENU(MESSAGE_INFO_RAID_ID, creature->GetGUID());
			break;

		case GOSSIP_ACTION_CHOSE_ROLE:
			//Le joueur doit choisir le rôle


			canTank = false;
			canHeal = false;


			//Tank
			if (player->getClass() == CLASS_WARRIOR
				|| player->getClass() == CLASS_DEATH_KNIGHT
				|| player->getClass() == CLASS_DRUID
				|| player->getClass() == CLASS_PALADIN
				) canTank = true;

			//Heal
			if (player->getClass() == CLASS_DRUID
				|| player->getClass() == CLASS_PRIEST
				|| player->getClass() == CLASS_PALADIN
				|| player->getClass() == CLASS_SHAMAN
				) canHeal = true;


			if (canTank)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, SAY_ROLE_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SUBSCRIBE_TANK);
			}

			if (canHeal)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, SAY_ROLE_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SUBSCRIBE_HEAL);
			}

			if (!canHeal && !canTank)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Annuler", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_CHOSE_ROLE);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, SAY_ROLE_DPS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_SUBSCRIBE_DPS);

			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;


		case GOSSIP_ACTION_SUBSCRIBE_TANK:
			sRGM2->addPlayerInQueue(player->GetGUID(), RGM<RAID_2>::ROLE_TANK);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_SUBSCRIBE_HEAL:
			sRGM2->addPlayerInQueue(player->GetGUID(), RGM<RAID_2>::ROLE_HEAL);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_SUBSCRIBE_DPS:
			sRGM2->addPlayerInQueue(player->GetGUID(), RGM<RAID_2>::ROLE_DPS);
			player->CLOSE_GOSSIP_MENU();
			break;

		}

		

		return true;
	}


};

class acces_donjons
{

public:

	void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
	{ 

		if (player->GetDungeonDifficulty() == Difficulty::DUNGEON_DIFFICULTY_HEROIC)
			return;

		std::function<void(Player*)> msgAndTele = [](Player* p){
			p->TeleportTo(1, -3632.899902f, -4363.140137f, 7.052210f, 1.049430f);
			p->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique");
		};

		newArea = player->GetAreaId();
		if ((newArea == 1337 || newArea == 2437 || newArea == 721 || newArea == 796) && !player->HasItemCount(100050))
			msgAndTele(player);
	}

};

void AddSC_script_npc_raid()
{
	new script_npc_raid();
	new acces_donjons();
}