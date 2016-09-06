

#include "ScriptMgr.h"
#include "Player.h"
#include <array>

#define ONE_GOLD 1000

#define AURA_DEFI 120145
#define MARQUE_DEFI 250050


enum AURAS_FROM_LOE
{
	ITEM_BUFF_SPELLPOWER = 696900,
	ITEM_BUFF_ATTACKPOWER = 696904,
	ITEM_BUFF_ATTACKSPEED = 696912,
	ITEM_BUFF_HP = 696908,
	ITEM_BUFF_ARMOR = 696920,
	ITEM_BUFF_RENVOI_SORT = 696916,
	ITEM_BUFF_REGENHP = 696928,
	ITEM_BUFF_MAGIC_RESIST = 696924,
	ITEM_BUFF_SPELLPOWER2 = 696901,
	ITEM_BUFF_ATTACKPOWER2 = 696905,
	ITEM_BUFF_ATTACKSPEED2 = 696913,
	ITEM_BUFF_HP2 = 696909,
	ITEM_BUFF_ARMOR2 = 696921,
	ITEM_BUFF_RENVOI_SORT2 = 696917,
	ITEM_BUFF_REGENHP2 = 696928,
	ITEM_BUFF_MAGIC_RESIST2 = 696925,
	ITEM_BUFF_SPELLPOWER3 = 696902,
	ITEM_BUFF_ATTACKPOWER3 = 696906,
	ITEM_BUFF_ATTACKSPEED3 = 696914,
	ITEM_BUFF_HP3 = 696910,
	ITEM_BUFF_ARMOR3 = 696922,
	ITEM_BUFF_RENVOI_SORT3 = 696918,
	ITEM_BUFF_REGENHP3 = 696929,
	ITEM_BUFF_MAGIC_RESIST3 = 696926,
	ITEM_BUFF_SPELLPOWER4 = 696903,
	ITEM_BUFF_ATTACKPOWER4 = 696907,
	ITEM_BUFF_ATTACKSPEED4 = 696915,
	ITEM_BUFF_HP4 = 696911,
	ITEM_BUFF_ARMOR4 = 696923,
	ITEM_BUFF_RENVOI_SORT4 = 696919,
	ITEM_BUFF_REGENHP4 = 696930,
	ITEM_BUFF_MAGIC_RESIST4 = 696927,
};

class script_unbuff : public PlayerScript
{

public:

	static const std::vector<uint32> spells;

public:

	script_unbuff() : PlayerScript("script_unbuff") 
	{
	}

	void OnMapChanged(Player* player)
	{ 
		for (int i = 0; i < script_unbuff::spells.size(); i++)
		{
			if (player->HasAura(script_unbuff::spells.at(i)))
			{
				player->RemoveAura(script_unbuff::spells.at(i));
			}

		}
	}

};

const std::vector<uint32> script_unbuff::spells = { 696927, 696930, 696912, 696919, 696923, 696911, 696915, 696907, 696903, 696926, 696929, 696918, 696922, 696910, 696914, 696906, 696902, 696925, 696928, 696917, 696921, 696909,
696913, 696905, 696901, 696924, 696928, 696916, 696920, 696908, 696912, 696904, 696900 };

//SPELL: commence à 300000
//jusqu'à 300007

class script_grades : public PlayerScript
{

public:

	script_grades() : PlayerScript("script_grades"){}

public:

	void OnPVPKill(Player* killer, Player* killed) {

		if (killer == killed)
			return;

		if (killer->GetQuestStatus(200239) == QUEST_STATUS_INCOMPLETE)
		{
			killer->KilledMonsterCredit(3010);
		}



		switch (killer->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS)){

		case 1: //Aspirant
		{
		
					killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Aspirant ! Vous recevez un titre et de l'or");

					const CharTitlesEntry* title = sCharTitlesStore.LookupEntry(210);

					if (title)
					{
						killer->SetTitle(title);
					}
					else
					{
						killer->GetSession()->SendAreaTriggerMessage("Vous n'avez pas le patch !");
					}
		
		}
			
			break;

		case 750: //Champion
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Champion ! Vous recevez un titre et de l'or");
			killer->SetTitle(sCharTitlesStore.LookupEntry(211));

			break;

		case 2000: //Conquérent
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Conquerent ! Vous recevez un titre et de l'or");
			killer->SetTitle(sCharTitlesStore.LookupEntry(212));

			break;

		case 5000: //Grand conquérent
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Grand Conquerent ! Vous recevez un titre et de l'or");
			killer->SetTitle(sCharTitlesStore.LookupEntry(213));

			break;

		case 8000: //Seigneur de guerre
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Seigneur de Guerre ! Vous recevez un titre et de l'or");
			killer->SetTitle(sCharTitlesStore.LookupEntry(218));

			break;

		case 12000: //Grand seigneur de guerre
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Grand Seigneur de Guerre ! Vous recevez un titre et de l'or");
			killer->SetTitle(sCharTitlesStore.LookupEntry(214));

			break;

		case 15000: //Chef de l'alliance / de la horde

			if (killer->getFaction() == ALLIANCE){
				killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Chef de l'alliance ! Vous recevez un titre et de l'or");
			}
			else {
				killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Chef de la horde ! Vous recevez un titre et de l'or");
				killer->SetTitle(sCharTitlesStore.LookupEntry(216));
			}

			break;

		case 80000: //Dieu de la guerre
			killer->GetSession()->SendAreaTriggerMessage("Felicitations, vous passez Dieu de la guerre ! Vous avez atteint le plus haut grade !");
			killer->SetTitle(sCharTitlesStore.LookupEntry(217));

			break;

		}

	}
};

class script_buff_grades : public PlayerScript
{

public:

	script_buff_grades() : PlayerScript("script_buff_grades")
	{

	}

public:

	void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 /*newArea*/){

		if (player->GetMapId() != 37)
		{
			player->DestroyItemCount(20558, 1000000000, true);
		}

		if (player->GetMap()->IsDungeon() && player->GetItemCount(34270))
		{
			player->AddAura(120146, player);

		}

		//HERE

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 80000 && !player->HasAura(300008))
		{
			player->AddAura(300008, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 15000 && !player->HasAura(300006))
		{
			player->AddAura(300006, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 12000 && !player->HasAura(300005))
		{
			player->AddAura(300005, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 8000 && !player->HasAura(300004))
		{
			player->AddAura(300004, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 5000 && !player->HasAura(300003))
		{
			player->AddAura(300003, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 2000 && !player->HasAura(300002))
		{
			player->AddAura(300002, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 750 && !player->HasAura(300001))
		{
			player->AddAura(300001, player);
			return;
		}

		if (player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS) >= 1 && !player->HasAura(300000))
		{
			player->AddAura(300000, player);
			return;
		}

	}

};

//Vendors list: 600021 -> 600028 pour les vendeurs list

class script_vendeur_jcj_grades : public CreatureScript
{
public:
	script_vendeur_jcj_grades() : CreatureScript("script_vendeur_jcj_grades") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->HasAura(300000, player->GetGUID())) {
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_01.:30|t Vendeur grade : I", GOSSIP_SENDER_MAIN, 1);
		}
		if (player->HasAura(300001, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_02:30|t Vendeur grade : II", GOSSIP_SENDER_MAIN, 2);
		}
		if (player->HasAura(300002, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Vendeur grade : III", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);

		}
		if (player->HasAura(300003, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_04:30|t Vendeur grade : IV", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);

		}
		if (player->HasAura(300004, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_05:30|t Vendeur grade : V", GOSSIP_SENDER_MAIN, 5);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);

		}
		if (player->HasAura(300005, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_06:30|t Vendeur grade : VI", GOSSIP_SENDER_MAIN, 6);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);

		}
		if (player->HasAura(300006, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_07:30|t Vendeur grade : VII", GOSSIP_SENDER_MAIN, 7);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);

		}
		if (player->HasAura(300008, player->GetGUID())) {

			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_13:30|t Vendeur grade : VIII", GOSSIP_SENDER_MAIN, 8);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_03:30|t Transmogrification Grade III", GOSSIP_SENDER_MAIN, 40);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_pvp_a_13:30|t Transmogrification Grade VIII", GOSSIP_SENDER_MAIN, 41);

		}
		player->SEND_GOSSIP_MENU(45250, creature->GetGUID());

		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {
			case 1:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600021);
				break;
			case 2:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600022);

				break;
			case 3:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600023);

				break;
			case 4:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600024);

				break;
			case 5:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600025);

				break;
			case 6:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600026);

				break;
			case 7:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600027);

				break;
			case 8:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600028);

				break;
			case 40:
				if (player->GetTeam() == HORDE)
				{
					player->GetSession()->SendListInventory(creature->GetGUID(), 12795);

				}
				else
					player->GetSession()->SendListInventory(creature->GetGUID(), 12785);

				break;
			}
		}
		return true;
	}
};


//DEFI
class script_defi : public PlayerScript
{
private :

	uint32 guidKilled;
public:

	script_defi() : PlayerScript("script_defi"){}

public:

	void OnPVPKill(Player* killer, Player* killed)
	{

		if (killed->GetGUID() == guidKilled)
		{
			return;
		}
		if (killer == killed)
		{
			return;
		}
		if (killer->GetSession()->GetRemoteAddress() == killed->GetSession()->GetRemoteAddress())
		{
			return;
		}
	
		if (killer->HasAura(AURA_DEFI))
		{
			guidKilled = killed->GetGUID();
			killer->AddItem(MARQUE_DEFI, 1);
		}
	}
};


class script_crash : public PlayerScript
{

public:

	script_crash() : PlayerScript("script_crash"){}

public:

	void OnSave(Player* player)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT * FROM crash_position WHERE playerGUID=%u", player->GetGUID());
		if (!result)
		{
			CharacterDatabase.PExecute("INSERT INTO crash_position VALUES(%u, %u, %f, %f, %f, %f)", player->GetGUID(), player->GetMap()->GetId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation());
		}
		else {
			CharacterDatabase.PQuery("UPDATE crash_position SET mapid=%u, x=%f, y=%f, z=%f, o=%f WHERE playerGUID=%u", player->GetMap()->GetId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), player->GetGUID());
		}
	}

};

void AddSC_script_grades()
{
	new script_grades();
	new script_buff_grades();
	new script_vendeur_jcj_grades();
	new script_defi();
	new script_unbuff();
	new script_crash();
}