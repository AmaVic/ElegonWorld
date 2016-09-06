#include "ScriptMgr.h"
#include "Group.h"
#include "RGM.h"
#include "pet.h"
#include "MastoHandler.h"

#define SAY_INFO "Qu'est-ce que la recherche de raid ?"
#define SAY_SUBSCRIBE "J'aimerais m'inscrire dans la recherche de raid"

#define SAY_ROLE_TANK "Tank"
#define SAY_ROLE_HEAL "Heal"
#define SAY_ROLE_DPS "DPS"

#define MESSAGE_INFO_RAID_ID 0 //à définir

#define RAID_ID 0
#define RAID_DIFFICULTY 0
#define PRICE_CHANGE_RACE 25
#define PRICE_CHANGE_NAME 10
#define PRICE_CHANGE_FACTION 50

enum GoAction
{
	GOSSIP_ACTION_INFO = 0,
	GOSSIP_ACTION_CHOSE_ROLE = 1,
	GOSSIP_ACTION_SUBSCRIBE_TANK = 2,
	GOSSIP_ACTION_SUBSCRIBE_HEAL = 3,
	GOSSIP_ACTION_SUBSCRIBE_DPS = 4
};

enum Price {

	price_charbon_constructor = 10, // Cout en bois pour charbon
	price_iron = 3, // Cout nombre de fer requis pour faire un 1 lingot
	price_iron_charbon = 5, // Cout nombre de charbon requis pour produire du fer  faire un 1 lingot
	price_gold = 3, // cout gold
	price_gold_charbon = 10, // cout gold charbon
	price_diamand = 3,
	price_diamand_charbon = 20,
	
	price_PA = 250,
	price_PS = 250,
	price_ENDU = 500,

	price_cloth_bad = 10,
	price_cloth_good = 10,
	price_cloth_realygood = 20,
	price_essence = 5,

	price_teinture_bad = 10,
	price_teinture_good = 5,
	price_teinture_realygood = 3
};

//experience requise level_house

uint32 level_1 = 1000;
uint32 level_2 = 2000;
uint32 level_3 = 4000;
uint32 level_4 = 8000;
uint32 level_5 = 12000;
uint32 level_6 = 8000;

uint32 entry_quest_1 = 1000001;
uint32 entry_quest_2 = 1000002;
uint32 entry_quest_3 = 1000003;

#define SPELL_AURA_PS 32483
#define SPELL_AURA_PA 30902
#define SPELL_AURA_ENDU 56257

/*-------------------------------*/
#define MAX_QUEST_DAILY 1
#define ITEM_QUEST_DAILY 100024


class NPC_Extension : public CreatureScript
{

public:
	NPC_Extension() : CreatureScript("NPC_Extension") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
		if (player->HasItemCount(436500, 1, true)) {
			player->GetSession()->SendAreaTriggerMessage("Vous avez deja une extension de maison");
			player->CLOSE_GOSSIP_MENU();

		}
		else {
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_townwatch:30|t Acheter une extension de maison", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Fermer", GOSSIP_SENDER_MAIN, 10);

			player->SEND_GOSSIP_MENU(26450, creature->GetGUID());
		}

		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1: {

						QueryResult result = CharacterDatabase.PQuery("SELECT * FROM house_buy WHERE owner = 0");

						if (!result) {

							player->GetSession()->SendAreaTriggerMessage("Aucune maison");
							return true;

						}

						Field* field = result->Fetch();
						uint32 houseId = field[0].GetUInt32();

						CharacterDatabase.PQuery("UPDATE house_buy SET owner = %u WHERE id = %u", player->GetGUID(), houseId);			
						player->CLOSE_GOSSIP_MENU();
						player->AddItem(436500, 1);


			}
			break;

			case 2: 
				player->CLOSE_GOSSIP_MENU();
				break;

			}

		}

		return true;

	}

};

class Spawn_quest_lutin : public GameObjectScript {


public:
	Spawn_quest_lutin() : GameObjectScript("Spawn_quest_lutin"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		if (player->GetQuestStatus(1000001) == QUEST_STATUS_INCOMPLETE) {
			player->ADD_GOSSIP_ITEM(2, "Ouvrir", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		}
		else {
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());

		}
		return true;

	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
			
					Creature * monster = player->SummonCreature(200006, player->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 30000);
					monster->AI()->AttackStart(player);
					player->AddItem(100022, urand(1, 100));
					player->CLOSE_GOSSIP_MENU();
					go->Delete();
				
			break;
			}

		}

		return true;
	}

};
class Teleporter_GOB : public GameObjectScript {


public:
	Teleporter_GOB() : GameObjectScript("Teleporter_GOB"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Teleporter", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->GetGroup()) {
					player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas etre en groupe pour aller dans cette zone");
					player->CLOSE_GOSSIP_MENU();
				}
				else {

					if (player->GetAreaId() == 3605) {
						player->TeleportTo(0, 211.476791f, -4190.152832f, 117.906288f, 0.882169f);
						player->CLOSE_GOSSIP_MENU();
					}

					if (player->GetAreaId() == 47) {

						player->TeleportTo(560, 2653.476318f, 664.076294f, 57.17390f, 1.041133f);
						player->CLOSE_GOSSIP_MENU();
						
					}
				}
				
			break;
			}

		}

		return true;
	}
	
};
class Teleporter_GOB2 : public GameObjectScript {


public:
	Teleporter_GOB2() : GameObjectScript("Teleporter_GOB2"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Teleporter", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				player->TeleportTo(571, 5759.994629f, 1015.972717f, 175.169876f, 3.150320f);

			break;
			}

		}

		return true;
	}

};





class Charbon_constructor : public GameObjectScript {


public :
 Charbon_constructor() : GameObjectScript("Charbon_constructor"){}
 
 bool OnGossipHello(Player* player, GameObject* go)
 {
	 std::stringstream charbon;
	 charbon << "Transformer " << price_charbon_constructor << " de bois en une unite de charbon";
	player->ADD_GOSSIP_ITEM(0, charbon.str(), GOSSIP_SENDER_MAIN, 1);
	 player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
	 return true;
 }
 bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
 {

	 if (sender == GOSSIP_SENDER_MAIN)
	 {
		 switch (uiAction)
		 {
		 case 1:

			 if (player->HasItemCount(100000, price_charbon_constructor, true)) {
				 player->AddItem(100011, 1);
				 player->DestroyItemCount(100000, price_charbon_constructor, true);
				 player->CLOSE_GOSSIP_MENU();
			 }
			 else {
				 player->CLOSE_GOSSIP_MENU();
				 player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de ressources");
			 }

		break;
		 }

	 }

	 return true;
 }

};
class well_lune : public GameObjectScript {


public:
	well_lune() : GameObjectScript("well_lune"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		player->ADD_GOSSIP_ITEM(0, "Jetez une marque de Marche Elegon World dans le puit.", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:

			break;
			}

		}

		return true;
	}

};
class NPC_House_blacksmith : public CreatureScript
{

public: 
	NPC_House_blacksmith() : CreatureScript("NPC_House_blacksmith") {}
	
	bool OnGossipHello(Player *player, Creature *creature)
	{
		std::stringstream forge_iron;
		forge_iron << "|cffFF0000Construire|r : Lingot de fer" << std::endl;
		forge_iron << "|cff2EFE2EComposants requis|r : " << price_iron << " minerais de fer" << " et " << price_iron_charbon << " unites de charbon";

		std::stringstream forge_gold;
		forge_gold << "|cffFF0000Construire|r : Lingot d'or" << std::endl;
		forge_gold << "|cff2EFE2EComposants requis|r : " << price_gold << " minerais d'or, " << price_gold_charbon << " unites de charbon et une boite a outils V1";

		std::stringstream forge_diamand;
		forge_diamand << "|cffFF0000Construire|r : Lingot de diamant" << std::endl;
		forge_diamand << "|cff2EFE2EComposants requis|r : " << price_diamand << " minerais de diamant, " << price_diamand_charbon << " unites de charbon et une boite a outils V2";

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "J'aimerais acheter quelque chose", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(0, forge_iron.str(), GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, forge_gold.str(), GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(0, forge_diamand.str(), GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(25001, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		QueryResult result_level = CharacterDatabase.PQuery("SELECT exp, lvl FROM house_player WHERE guid_player = %u", player->GetGUID());

		if (!result_level)
		{
			return false;
		}

		Field *fields = result_level->Fetch();
		uint32 exp_hous = fields[0].GetUInt32();
		uint32 lvl = fields[1].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:

				if (player->HasItemCount(100001, price_iron, true) && player->HasItemCount(100011, price_iron_charbon, true)) { // Si le joueur à au moins les composants requis.
					
					if (player->HasItemCount(100009, 1, true)) {
						player->SetMoney(player->GetMoney() + 100000);

						player->AddItem(100012, 1);
						player->DestroyItemCount(100001, price_iron, true);
						player->DestroyItemCount(100011, price_iron_charbon, true);
						player->DestroyItemCount(100009, 1, true);
						player->CLOSE_GOSSIP_MENU();

					}
					else {
						player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de sceau d'eau");
					}
					
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les composants requis");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 2:

				if (player->HasItemCount(100002, price_gold, true) && player->HasItemCount(100011, price_gold_charbon, true)) { // Si le joueur à au moins les composants requis.

					if (player->HasItemCount(100009, 1, true)) {
						if (player->HasItemCount(100013, 1, true)) {
							player->SetMoney(player->GetMoney() + 100000);
							player->AddItem(100015, 1);
							player->DestroyItemCount(100002, price_gold, true);
							player->DestroyItemCount(100011, price_gold_charbon, true);
							player->DestroyItemCount(100009, 1, true);
							player->CLOSE_GOSSIP_MENU();
						}
						else {
							player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les outils requis");
						}

					}
					else {
						player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de sceau d'eau");
					}

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les composants requis");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 3:

				if (player->HasItemCount(100003, price_diamand, true) && player->HasItemCount(100011, price_diamand_charbon, true)) { // Si le joueur à au moins les composants requis.

					if (player->HasItemCount(100009, 1, true)) {
						if (player->HasItemCount(100014, 1, true)) {
							player->SetMoney(player->GetMoney() + 100000);
							player->AddItem(100016, 1);
							player->DestroyItemCount(100003, price_diamand, true);
							player->DestroyItemCount(100011, price_diamand_charbon, true);
							player->DestroyItemCount(100009, 1, true);
							player->CLOSE_GOSSIP_MENU();
						}
						else {
							player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les outils requis");
						}

					}
					else {
						player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de sceau d'eau");
					}

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les composants requis");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 4:
				if (lvl == 5) {
					player->GetSession()->SendListInventory(creature->GetGUID());

				}
				else {
				
					player->GetSession()->SendAreaTriggerMessage("Votre maison doit etre au moins niveau 5");
					player->CLOSE_GOSSIP_MENU();
				}

				
				break;

			}
		}
		return true;
	}




};
class NPC_House_taillor : public CreatureScript
{

public:
	NPC_House_taillor() : CreatureScript("NPC_House_taillor") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		std::stringstream clothe_bad;
		clothe_bad << "|cffFF0000Construire|r : Tissu de bonne qualite" << std::endl;
		clothe_bad << "|cff2EFE2EComposants requis|r : " << price_cloth_bad << " tissus dechires" << " et 1 teinture normale";

		std::stringstream clothe_energize;
		clothe_energize << "|cffFF0000Construire|r : Etoffe d'energie pure" << std::endl;
		clothe_energize << "|cff2EFE2EComposants requis|r : " << price_cloth_realygood << " tissus de bonne qualite, " << price_essence << " essences de flamme, " << price_essence << " essences de terre, " << price_essence << " essences d'air, " << price_essence << " essences d'eau et " << price_teinture_realygood << " teintures d'energie pure";

		std::stringstream clothe_tenebre;
		clothe_tenebre << "|cffFF0000Construire|r : Etoffe des tenebres" << std::endl;
		clothe_tenebre << "|cff2EFE2EComposants requis|r : " << price_cloth_good << " tissus de bonne qualite, " << price_essence << " essences de flamme, " << price_essence << " essences de terre, " << price_essence << " essences d'air, " << price_essence << " essences d'eau et " << price_teinture_good << " teintures des tenebres";


		player->ADD_GOSSIP_ITEM(0, clothe_bad.str() , GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, clothe_energize.str(), GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(0, clothe_tenebre.str(), GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "J'aimerais acheter quelques choses", GOSSIP_SENDER_MAIN, 4);
		player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		QueryResult result_level = CharacterDatabase.PQuery("SELECT exp, lvl FROM house_player WHERE guid_player = %u", player->GetGUID());

		Field *fields = result_level->Fetch();
		uint32 exp_hous = fields[0].GetUInt32();
		uint32 lvl = fields[1].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(100004, price_cloth_bad, true) && player->HasItemCount(100025, 1, true)) {

					player->CLOSE_GOSSIP_MENU();
					player->AddItem(100007, 1);
					player->DestroyItemCount(100004, price_cloth_bad, true);
					player->DestroyItemCount(100025, 1, true);
					player->SetMoney(player->GetMoney() + 100000);


				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
			break;
			case 2:
				if (player->HasItemCount(100007, price_cloth_realygood, true) &&
					player->HasItemCount(100018, price_essence, true) &&
					player->HasItemCount(100019, price_essence, true) &&
					player->HasItemCount(100020, price_essence, true) &&
					player->HasItemCount(100021, price_essence, true) && 
					player->HasItemCount(100027, price_teinture_realygood, true)) {
					player->DestroyItemCount(100007, price_cloth_realygood, true);
					player->DestroyItemCount(100018, price_essence, true);
					player->DestroyItemCount(100019, price_essence, true);
					player->DestroyItemCount(100020, price_essence, true);
					player->DestroyItemCount(100021, price_essence, true);
					player->DestroyItemCount(100027, price_teinture_realygood, true);
					player->AddItem(100006, 1);
					player->SetMoney(player->GetMoney() + 100000);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			case 3:
				if (player->HasItemCount(100007, price_cloth_good, true) &&player->HasItemCount(100018, price_essence, true) &&
					player->HasItemCount(100019, price_essence, true) &&
					player->HasItemCount(100020, price_essence, true) &&
					player->HasItemCount(100021, price_essence, true) &&
					player->HasItemCount(100026, price_teinture_realygood, true)) {
					player->DestroyItemCount(100007, price_cloth_good, true);
					player->DestroyItemCount(100018, price_essence, true);
					player->DestroyItemCount(100019, price_essence, true);
					player->DestroyItemCount(100020, price_essence, true);
					player->DestroyItemCount(100021, price_essence, true);
					player->DestroyItemCount(100026, price_teinture_good, true);
					player->AddItem(100005, 1);
					player->SetMoney(player->GetMoney() + 100000);
					player->CLOSE_GOSSIP_MENU();

				}
				else {

					player->GetSession()->SendAreaTriggerMessage("Pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			
			case 4:
				if (lvl == 0) {
					player->GetSession()->SendAreaTriggerMessage("Votre maison doit etre au moins niveau 2");
					player->CLOSE_GOSSIP_MENU();
				}
				else if (lvl > 1) {
					player->GetSession()->SendListInventory(creature->GetGUID());
				}
				break;

			}
		}
		return true;
	}



};
class NPC_House_main : public CreatureScript
{

public:
	NPC_House_main() : CreatureScript("NPC_House_main") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		QueryResult result = CharacterDatabase.PQuery("SELECT guid_player, exp, lvl FROM house_player WHERE guid_player = %u", player->GetGUID());

		Field *fields = result->Fetch();
		uint32 guid_player = fields[0].GetUInt32();
		uint32 exp = fields[1].GetUInt32();
		uint32 lvl = fields[2].GetUInt32();

		std::stringstream info_house;
		info_house << "-----------|cff3ADF00Informations|r---------" << std::endl;
		info_house << "Niveau de votre maison : " << lvl << std::endl;
		info_house << "Total d'experience : " << exp << std::endl;
		(lvl == 5 ? info_house << "Votre maison est au niveau maximum." << std::endl : info_house << std::endl);
		player->ADD_GOSSIP_ITEM(0, info_house.str(), GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Monter de niveau", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Echanger mes ressources", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Acheter des caracteristiques", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Qu'est-ce que vous avez a me vendre ?", GOSSIP_SENDER_MAIN, 6);
		player->SEND_GOSSIP_MENU(25000, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		QueryResult result_level = CharacterDatabase.PQuery("SELECT exp, lvl FROM house_player WHERE guid_player = %u", player->GetGUID());

		Field *fields = result_level->Fetch();
		uint32 exp_hous = fields[0].GetUInt32();
		uint32 lvl = fields[1].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 2:

				if (lvl == 0) {
					if (exp_hous >= level_1) {
						player->CLOSE_GOSSIP_MENU();
						CharacterDatabase.PQuery("UPDATE house_player SET lvl = lvl + 1 WHERE guid_player = %u", player->GetGUID());
						player->GetSession()->SendAreaTriggerMessage("Excellent, votre maison est maintenant niveau 1 !");
						
					}
					else {
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendAreaTriggerMessage("Pas assez d'experience !");

					}
				}
				else if (lvl == 1) {
					if (exp_hous >= level_2) {
						player->CLOSE_GOSSIP_MENU();
						CharacterDatabase.PQuery("UPDATE house_player SET lvl = lvl + 1 WHERE guid_player = %u", player->GetGUID());
						player->GetSession()->SendAreaTriggerMessage("Excellent votre maison est maintenant niveau 2 !");
					}
					else {
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendAreaTriggerMessage("Pas assez d'experience !");
					}
				}
				else if (lvl == 2) {
					if (exp_hous >= level_3) {
						player->CLOSE_GOSSIP_MENU();
						CharacterDatabase.PQuery("UPDATE house_player SET lvl = lvl + 1 WHERE guid_player = %u", player->GetGUID());
						player->GetSession()->SendAreaTriggerMessage("Excellent, votre maison est maintenant niveau 3 !");

					}
					else {
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendAreaTriggerMessage("Pas assez d'experience !");
					}
				}
				else if (lvl == 3) {
					if (exp_hous >= level_4) {
						player->CLOSE_GOSSIP_MENU();
						CharacterDatabase.PQuery("UPDATE house_player SET lvl = lvl + 1 WHERE guid_player = %u", player->GetGUID());
						player->GetSession()->SendAreaTriggerMessage("Excellent, votre maison est maintenant niveau 4 !");

					}
					else {
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendAreaTriggerMessage("Pas assez d'experience!");
					}
				}

				else if (lvl == 4) {
					if (exp_hous >= level_5) {
						player->CLOSE_GOSSIP_MENU();
						CharacterDatabase.PQuery("UPDATE house_player SET lvl = lvl + 1 WHERE guid_player = %u", player->GetGUID());
						player->GetSession()->SendAreaTriggerMessage("Excellent, votre maison a atteint le niveau maximum: 5");

					}
					else {
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendAreaTriggerMessage("Pas assez d'experience !");
					}
				}
				else {
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Niveau de maison au maximum !");
				}
	

			break;

			case 3:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bois x250 + |cff00FF00 10 points d'experience", GOSSIP_SENDER_MAIN, 10);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Lingot de fer + |cff00FF00 10 points d'experience", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Lingot d'or + |cff00FF00 20 points d'experience", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Lingot de diamant + |cff00FF00 40 points d'experience", GOSSIP_SENDER_MAIN, 13);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Tissus de bonne qualite x10 + |cff00FF00 10 points d'experience", GOSSIP_SENDER_MAIN, 14);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Etoffe de tenebres + |cff00FF00 40 points d'experience", GOSSIP_SENDER_MAIN, 15);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Etoffe d'energie pure + |cff00FF00 80 points d'experience", GOSSIP_SENDER_MAIN, 16);
				player->CLOSE_GOSSIP_MENU();
				player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());

			break;
			case 4:
				switch (urand(0, 2)) {
					
				case 0: {
							if (player->HasItemCount(ITEM_QUEST_DAILY, MAX_QUEST_DAILY, false)) {
								player->CLOSE_GOSSIP_MENU();
								player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez plus faire de quetes aujourd'hui !");
								break;
							}
							else {

								Quest const* quest = sObjectMgr->GetQuestTemplate(entry_quest_1);
								Object * giver = creature->ToCreature();
								player->AddQuest(quest, giver);
								player->AddItem(ITEM_QUEST_DAILY, MAX_QUEST_DAILY);
								player->CLOSE_GOSSIP_MENU();
							}

				}
					
					break;
				case 1:
				{
						  if (player->HasItemCount(ITEM_QUEST_DAILY, MAX_QUEST_DAILY, false)) {
							  player->CLOSE_GOSSIP_MENU();
							  player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez plus faire de quetes aujourd'hui !");
							  break;
						  }
						  else {

							  Quest const* quest = sObjectMgr->GetQuestTemplate(entry_quest_2);
							  Object * giver = creature->ToCreature();
							  player->AddQuest(quest, giver);
							  player->AddItem(ITEM_QUEST_DAILY, MAX_QUEST_DAILY);
							  player->CLOSE_GOSSIP_MENU();
						  }

					
					break;

				case 2: {
							if (player->HasItemCount(ITEM_QUEST_DAILY, MAX_QUEST_DAILY, false)) {
								player->CLOSE_GOSSIP_MENU();
								player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez plus faire de quetes aujourd'hui !");
								break;
							}
							else {

								Quest const* quest = sObjectMgr->GetQuestTemplate(entry_quest_3);
								Object * giver = creature->ToCreature();
								player->AddQuest(quest, giver);
								player->AddItem(ITEM_QUEST_DAILY, MAX_QUEST_DAILY);
								player->CLOSE_GOSSIP_MENU();
							}

							break;
				}

					}
				

				}
			break;
			case 6:
				player->GetSession()->SendListInventory(creature->GetGUID());
				break;

			case 25:
				if (player->HasItemCount(100023, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 1000);
					player->SetMoney(player->GetMoney() + 100000);

					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 500 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100023, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 10:
				if (player->HasItemCount(100000, 250, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 100);
					player->SetMoney(player->GetMoney() + 100000);

					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 10 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100000, 250, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}	
		break;
			case 11:
				if (player->HasItemCount(100012, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 100);
				player->SetMoney(player->GetMoney() + 100000);

					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 10 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100012, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 12:
				if (player->HasItemCount(100015, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 200);
					player->SetMoney(player->GetMoney() + 100000);
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 20 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100015, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 13:
				if (player->HasItemCount(100016, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 400);
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 40 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100016, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 14:
				if (player->HasItemCount(100007, 10, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 100);
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 10 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100007, 10, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 15:
				if (player->HasItemCount(100005, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 400);
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 40 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->DestroyItemCount(100005, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 16:
				if (player->HasItemCount(100006, 1, true)) {
					player->SetReputation(1156, player->GetReputation(1156) + 400);
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 80 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Un grand merci pour votre achat !");
					player->CLOSE_GOSSIP_MENU();
					player->DestroyItemCount(100006, 1, true);
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 5:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Ces ameliorations sont permanentes. Apres achat, merci de vous deconnecter puis de vous reconnecter pour que la modification soit prise en compte.", GOSSIP_SENDER_MAIN, 52142);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_Quests_Completed_08:30|t Acheter 2% de puissance d'attaque (250 Points d'experiences)", GOSSIP_SENDER_MAIN, 17);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_Quests_Completed_08:30|t Acheter 2% d'endurance (500 Points d'experiences)", GOSSIP_SENDER_MAIN, 18);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_Quests_Completed_08:30|t Acheter 2% de degat des sorts (250 Points d'experiences)", GOSSIP_SENDER_MAIN, 19);
				player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());

				break;
			case 17:
				if (exp_hous >= price_PA) {
					CharacterDatabase.PQuery("UPDATE house_player SET PA = PA + 1 WHERE guid_player = %u", player->GetGUID());	
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp - 250 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Effectuez une deco/reco et le tour est joue !");
					player->CLOSE_GOSSIP_MENU();
			}
				else {

					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;

			case 18:
				if (exp_hous >= price_ENDU) {
					CharacterDatabase.PQuery("UPDATE house_player SET ENDU = ENDU + 1 WHERE guid_player = %u", player->GetGUID());
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp - 500 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Effectuez une deco/reco et le tour est joue !");
					player->CLOSE_GOSSIP_MENU();
				}
				else {

					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 19:
				if (exp_hous >= price_PS) {
					CharacterDatabase.PQuery("UPDATE house_player SET PS = PS + 1 WHERE guid_player = %u", player->GetGUID());
					CharacterDatabase.PQuery("UPDATE house_player SET exp = exp - 250 WHERE guid_player = %u", player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Effectuez une deco/reco et le tour est joue !");
					player->CLOSE_GOSSIP_MENU();
				}
				else {

					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de ressources !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;

				
			}
		}
		return true;
	}



};


class On_Login_Housing : public PlayerScript
{
private : 

	bool isStarted;

public:
	On_Login_Housing() : PlayerScript("On_Login_Housing") {}

	void OnLogin(Player* player, bool firstLogin) {

		if (firstLogin) {
			switch (player->getClass())
			{
				player->CastSpell(player, 63624);

			case CLASS_WARRIOR:

				player->learnSpell(7384, false);
				player->learnSpell(47436, false);
				player->learnSpell(47450, false);
				player->learnSpell(11578, false);
				player->learnSpell(47465, false);
				player->learnSpell(47502, false);
				player->learnSpell(34428, false);
				player->learnSpell(1715, false);
				player->learnSpell(2687, false);
				player->learnSpell(71, false);
				player->learnSpell(7386, false);
				player->learnSpell(355, false);
				player->learnSpell(72, false);
				player->learnSpell(47437, false);
				player->learnSpell(57823, false);
				player->learnSpell(694, false);
				player->learnSpell(2565, false);
				player->learnSpell(676, false);
				player->learnSpell(47520, false);
				player->learnSpell(20230, false);
				player->learnSpell(12678, false);
				player->learnSpell(47471, false);
				player->learnSpell(1161, false);
				player->learnSpell(871, false);
				player->learnSpell(2458, false);
				player->learnSpell(20252, false);
				player->learnSpell(47475, false);
				player->learnSpell(18499, false);
				player->learnSpell(1680, false);
				player->learnSpell(6552, false);
				player->learnSpell(47488, false);
				player->learnSpell(1719, false);
				player->learnSpell(23920, false);
				player->learnSpell(47440, false);
				player->learnSpell(3411, false);
				player->learnSpell(64382, false);
				player->learnSpell(55694, false);
				player->learnSpell(57755, false);
				player->learnSpell(42459, false);
				player->learnSpell(750, false);
				player->learnSpell(5246, false);
				player->learnSpell(3127, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(12294)) // Mortal Strike.
					player->learnSpell(47486, false);
				if (player->HasSpell(20243)) // Devastate.
					player->learnSpell(47498, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_PALADIN:
				player->learnSpell(3127, false);
				player->learnSpell(19746, false);
				player->learnSpell(19752, false);
				player->learnSpell(750, false);
				player->learnSpell(48942, false);
				player->learnSpell(48782, false);
				player->learnSpell(48932, false);
				player->learnSpell(20271, false);
				player->learnSpell(498, false);
				player->learnSpell(10308, false);
				player->learnSpell(1152, false);
				player->learnSpell(10278, false);
				player->learnSpell(48788, false);
				player->learnSpell(53408, false);
				player->learnSpell(48950, false);
				player->learnSpell(48936, false);
				player->learnSpell(31789, false);
				player->learnSpell(62124, false);
				player->learnSpell(54043, false);
				player->learnSpell(25780, false);
				player->learnSpell(1044, false);
				player->learnSpell(20217, false);
				player->learnSpell(48819, false);
				player->learnSpell(48801, false);
				player->learnSpell(48785, false);
				player->learnSpell(5502, false);
				player->learnSpell(20164, false);
				player->learnSpell(10326, false);
				player->learnSpell(1038, false);
				player->learnSpell(53407, false);
				player->learnSpell(48943, false);
				player->learnSpell(20165, false);
				player->learnSpell(48945, false);
				player->learnSpell(642, false);
				player->learnSpell(48947, false);
				player->learnSpell(20166, false);
				player->learnSpell(4987, false);
				player->learnSpell(48806, false);
				player->learnSpell(6940, false);
				player->learnSpell(48817, false);
				player->learnSpell(48934, false);
				player->learnSpell(48938, false);
				player->learnSpell(25898, false);
				player->learnSpell(32223, false);
				player->learnSpell(31884, false);
				player->learnSpell(54428, false);
				player->learnSpell(61411, false);
				player->learnSpell(53601, false);
				player->learnSpell(33388, false);
				player->learnSpell(33391, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(20925)) // Holy Shield.
					player->learnSpell(48952, false);
				if (player->HasSpell(31935)) // Avenger's Shield.
					player->learnSpell(48827, false);
				if (player->HasSpell(20911)) // Blessing of Sanctuary.
					player->learnSpell(25899, false); // Greater Blessing of Sanctuary.
				if (player->HasSpell(20473)) // Holy Shock.
					player->learnSpell(48825, false);
				if (player->GetTeam() == ALLIANCE)
				{
					player->learnSpell(31801, false); // Seal of Vengeance.
					player->learnSpell(13819, false); // Warhorde.
					player->learnSpell(23214, false); // Charger.
				}
				if (player->GetTeam() == HORDE)
				{
					player->learnSpell(53736, false); // Seal of Corruption.
					player->learnSpell(34769, false); // Thalassian Warhorse.
					player->learnSpell(34767, false); // Thalassian Charger.
				}
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_HUNTER:
				player->learnSpell(3043, false);
				player->learnSpell(3127, false);
				player->learnSpell(3045, false);
				player->learnSpell(3034, false);
				player->learnSpell(8737, false);
				player->learnSpell(1494, false);
				player->learnSpell(13163, false);
				player->learnSpell(48996, false);
				player->learnSpell(49001, false);
				player->learnSpell(49045, false);
				player->learnSpell(53338, false);
				player->learnSpell(5116, false);
				player->learnSpell(27044, false);
				player->learnSpell(883, false);
				player->learnSpell(2641, false);
				player->learnSpell(6991, false);
				player->learnSpell(982, false);
				player->learnSpell(1515, false);
				player->learnSpell(19883, false);
				player->learnSpell(20736, false);
				player->learnSpell(48990, false);
				player->learnSpell(2974, false);
				player->learnSpell(6197, false);
				player->learnSpell(1002, false);
				player->learnSpell(14327, false);
				player->learnSpell(5118, false);
				player->learnSpell(49056, false);
				player->learnSpell(53339, false);
				player->learnSpell(49048, false);
				player->learnSpell(19884, false);
				player->learnSpell(34074, false);
				player->learnSpell(781, false);
				player->learnSpell(14311, false);
				player->learnSpell(1462, false);
				player->learnSpell(19885, false);
				player->learnSpell(19880, false);
				player->learnSpell(13809, false);
				player->learnSpell(13161, false);
				player->learnSpell(5384, false);
				player->learnSpell(1543, false);
				player->learnSpell(19878, false);
				player->learnSpell(49067, false);
				player->learnSpell(3034, false);
				player->learnSpell(13159, false);
				player->learnSpell(19882, false);
				player->learnSpell(58434, false);
				player->learnSpell(49071, false);
				player->learnSpell(49052, false);
				player->learnSpell(19879, false);
				player->learnSpell(19263, false);
				player->learnSpell(19801, false);
				player->learnSpell(34026, false);
				player->learnSpell(34600, false);
				player->learnSpell(34477, false);
				player->learnSpell(61006, false);
				player->learnSpell(61847, false);
				player->learnSpell(53271, false);
				player->learnSpell(60192, false);
				player->learnSpell(62757, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(19386)) // Wyvern Sting.
					player->learnSpell(49012, false);
				if (player->HasSpell(53301)) // Explosive Shot.
					player->learnSpell(60053, false);
				if (player->HasSpell(19306)) // Counter Attack.
					player->learnSpell(48999, false);
				if (player->HasSpell(19434)) // Aimed Shot.
					player->learnSpell(49050, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_MAGE:
				player->learnSpell(42921, false);
				player->learnSpell(42842, false);
				player->learnSpell(42995, false);
				player->learnSpell(42833, false);
				player->learnSpell(27090, false);
				player->learnSpell(33717, false);
				player->learnSpell(42873, false);
				player->learnSpell(42846, false);
				player->learnSpell(12826, false);
				player->learnSpell(28271, false);
				player->learnSpell(61780, false);
				player->learnSpell(61721, false);
				player->learnSpell(28272, false);
				player->learnSpell(42917, false);
				player->learnSpell(43015, false);
				player->learnSpell(130, false);
				player->learnSpell(42926, false);
				player->learnSpell(43017, false);
				player->learnSpell(475, false);
				player->learnSpell(1953, false);
				player->learnSpell(42940, false);
				player->learnSpell(12051, false);
				player->learnSpell(43010, false);
				player->learnSpell(43020, false);
				player->learnSpell(43012, false);
				player->learnSpell(42859, false);
				player->learnSpell(2139, false);
				player->learnSpell(42931, false);
				player->learnSpell(42985, false);
				player->learnSpell(43008, false);
				player->learnSpell(45438, false);
				player->learnSpell(43024, false);
				player->learnSpell(43002, false);
				player->learnSpell(43046, false);
				player->learnSpell(42897, false);
				player->learnSpell(42914, false);
				player->learnSpell(66, false);
				player->learnSpell(58659, false);
				player->learnSpell(30449, false);
				player->learnSpell(42956, false);
				player->learnSpell(47610, false);
				player->learnSpell(61316, false);
				player->learnSpell(61024, false);
				player->learnSpell(55342, false);
				player->learnSpell(53142, false);
				player->learnSpell(7301, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->GetTeam() == ALLIANCE)
				{
					player->learnSpell(32271, false); // Teleport: Exodar.
					player->learnSpell(49359, false); // Teleport: Theramore.
					player->learnSpell(3565, false); // Teleport: Darnassus.
					player->learnSpell(33690, false); // Teleport: Shattrath.
					player->learnSpell(3562, false); // Teleport: Ironforge.
					player->learnSpell(3561, false); // Teleport: Stormwind.
					player->learnSpell(53140, false); // Teleport: Dalaran.
					player->learnSpell(53142, false); // Portal: Dalaran.
					player->learnSpell(10059, false); // Portal: Stormwind.
					player->learnSpell(11419, false); // Portal: Darnassus.
					player->learnSpell(32266, false); // Portal: Exodar.
					player->learnSpell(11416, false); // Portal: Ironforge.
					player->learnSpell(33691, false); // Portal: Shattrath.
					player->learnSpell(49360, false); // Portal: Theramore.
				}
				if (player->GetTeam() == HORDE)
				{
					player->learnSpell(3567, false); // Teleport: Orgrimmar.
					player->learnSpell(35715, false); // Teleport: Shattrath.
					player->learnSpell(3566, false); // Teleport: Thunder Bluff.
					player->learnSpell(49358, false); // Teleport: Stonard.
					player->learnSpell(32272, false); // Teleport: Silvermoon.
					player->learnSpell(3563, false); // Teleport: Undercity.
					player->learnSpell(53140, false); // Teleport: Dalaran.
					player->learnSpell(53142, false); // Portal: Dalaran.
					player->learnSpell(11417, false); // Portal: Orgrimmar.
					player->learnSpell(35717, false); // Portal: Shattrath.
					player->learnSpell(32267, false); // Portal: Silvermoon.
					player->learnSpell(49361, false); // Portal: Stonard.
					player->learnSpell(11420, false); // Portal: Thunder Bluff.
					player->learnSpell(11418, false); // Portal: Undercity.
				}
				if (player->HasSpell(11366)) // Pyroblast.
					player->learnSpell(42891, false);
				if (player->HasSpell(11426)) // Ice Barrier.
					player->learnSpell(43039, false);
				if (player->HasSpell(44457)) // Living Bomb.
					player->learnSpell(55360, false);
				if (player->HasSpell(31661)) // Dragon's Breath.
					player->learnSpell(42950, false);
				if (player->HasSpell(11113)) // Blast Wave.
					player->learnSpell(42945, false);
				if (player->HasSpell(44425)) // Arcane Barrage.
					player->learnSpell(44781, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_WARLOCK:
				player->learnSpell(696, false);
				player->learnSpell(47811, false);
				player->learnSpell(47809, false);
				player->learnSpell(688, false);
				player->learnSpell(47813, false);
				player->learnSpell(50511, false);
				player->learnSpell(57946, false);
				player->learnSpell(47864, false);
				player->learnSpell(6215, false);
				player->learnSpell(47878, false);
				player->learnSpell(47855, false);
				player->learnSpell(697, false);
				player->learnSpell(47856, false);
				player->learnSpell(47857, false);
				player->learnSpell(5697, false);
				player->learnSpell(47884, false);
				player->learnSpell(47815, false);
				player->learnSpell(47889, false);
				player->learnSpell(47820, false);
				player->learnSpell(698, false);
				player->learnSpell(712, false);
				player->learnSpell(126, false);
				player->learnSpell(5138, false);
				player->learnSpell(5500, false);
				player->learnSpell(11719, false);
				player->learnSpell(132, false);
				player->learnSpell(60220, false);
				player->learnSpell(18647, false);
				player->learnSpell(61191, false);
				player->learnSpell(47823, false);
				player->learnSpell(691, false);
				player->learnSpell(47865, false);
				player->learnSpell(47891, false);
				player->learnSpell(47888, false);
				player->learnSpell(17928, false);
				player->learnSpell(47860, false);
				player->learnSpell(47825, false);
				player->learnSpell(1122, false);
				player->learnSpell(47867, false);
				player->learnSpell(18540, false);
				player->learnSpell(47893, false);
				player->learnSpell(47838, false);
				player->learnSpell(29858, false);
				player->learnSpell(58887, false);
				player->learnSpell(47836, false);
				player->learnSpell(61290, false);
				player->learnSpell(48018, false);
				player->learnSpell(48020, false);
				player->learnSpell(33388, false);
				player->learnSpell(33391, false);
				player->learnSpell(23161, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(17877)) // Shadowburn.
					player->learnSpell(47827, false);
				if (player->HasSpell(30283)) // Shadowfury.
					player->learnSpell(47847, false);
				if (player->HasSpell(30108)) // Unstable Affliction.
					player->learnSpell(47843, false);
				if (player->HasSpell(50796)) // Chaos Bolt.
					player->learnSpell(59172, false);
				if (player->HasSpell(48181)) // Haunt.
					player->learnSpell(59164, false);
				if (player->HasSpell(18220)) // Dark Pact.
					player->learnSpell(59092, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_ROGUE:
				player->learnSpell(3127, false);
				player->learnSpell(42459, false);
				player->learnSpell(48668, false);
				player->learnSpell(48638, false);
				player->learnSpell(1784, false);
				player->learnSpell(48657, false);
				player->learnSpell(921, false);
				player->learnSpell(1776, false);
				player->learnSpell(26669, false);
				player->learnSpell(51724, false);
				player->learnSpell(6774, false);
				player->learnSpell(11305, false);
				player->learnSpell(1766, false);
				player->learnSpell(48676, false);
				player->learnSpell(48659, false);
				player->learnSpell(1804, false);
				player->learnSpell(8647, false);
				player->learnSpell(48691, false);
				player->learnSpell(51722, false);
				player->learnSpell(48672, false);
				player->learnSpell(1725, false);
				player->learnSpell(26889, false);
				player->learnSpell(2836, false);
				player->learnSpell(1833, false);
				player->learnSpell(1842, false);
				player->learnSpell(8643, false);
				player->learnSpell(2094, false);
				player->learnSpell(1860, false);
				player->learnSpell(57993, false);
				player->learnSpell(48674, false);
				player->learnSpell(31224, false);
				player->learnSpell(5938, false);
				player->learnSpell(57934, false);
				player->learnSpell(51723, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(16511)) // Hemorrhage.
					player->learnSpell(48660, false);
				if (player->HasSpell(1329)) // Mutilate.
					player->learnSpell(48666, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_PRIEST:
				player->learnSpell(528, false);
				player->learnSpell(2053, false);
				player->learnSpell(48161, false);
				player->learnSpell(48123, false);
				player->learnSpell(48125, false);
				player->learnSpell(48066, false);
				player->learnSpell(586, false);
				player->learnSpell(48068, false);
				player->learnSpell(48127, false);
				player->learnSpell(48171, false);
				player->learnSpell(48168, false);
				player->learnSpell(10890, false);
				player->learnSpell(6064, false);
				player->learnSpell(988, false);
				player->learnSpell(48300, false);
				player->learnSpell(6346, false);
				player->learnSpell(48071, false);
				player->learnSpell(48135, false);
				player->learnSpell(48078, false);
				player->learnSpell(453, false);
				player->learnSpell(10955, false);
				player->learnSpell(10909, false);
				player->learnSpell(8129, false);
				player->learnSpell(48073, false);
				player->learnSpell(605, false);
				player->learnSpell(48072, false);
				player->learnSpell(48169, false);
				player->learnSpell(552, false);
				player->learnSpell(1706, false);
				player->learnSpell(48063, false);
				player->learnSpell(48162, false);
				player->learnSpell(48170, false);
				player->learnSpell(48074, false);
				player->learnSpell(48158, false);
				player->learnSpell(48120, false);
				player->learnSpell(34433, false);
				player->learnSpell(48113, false);
				player->learnSpell(32375, false);
				player->learnSpell(64843, false);
				player->learnSpell(64901, false);
				player->learnSpell(53023, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(34914)) // Vampiric Touch.
					player->learnSpell(48160, false);
				if (player->HasSpell(47540)) // Penance.
					player->learnSpell(53007, false);
				if (player->HasSpell(724)) // Lightwell.
					player->learnSpell(48087, false);
				if (player->HasSpell(19236)) // Desperate Prayer.
					player->learnSpell(48173, false);
				if (player->HasSpell(34861)) // Circle of Healing.
					player->learnSpell(48089, false);
				if (player->HasSpell(15407)) // Mind Flay.
					player->learnSpell(48156, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_DEATH_KNIGHT:
				player->learnSpell(3127, false);
				player->learnSpell(50842, false);
				player->learnSpell(49941, false);
				player->learnSpell(49930, false);
				player->learnSpell(47476, false);
				player->learnSpell(45529, false);
				player->learnSpell(3714, false);
				player->learnSpell(56222, false);
				player->learnSpell(48743, false);
				player->learnSpell(48263, false);
				player->learnSpell(49909, false);
				player->learnSpell(47528, false);
				player->learnSpell(45524, false);
				player->learnSpell(48792, false);
				player->learnSpell(57623, false);
				player->learnSpell(56815, false);
				player->learnSpell(47568, false);
				player->learnSpell(49895, false);
				player->learnSpell(50977, false);
				player->learnSpell(49576, false);
				player->learnSpell(49921, false);
				player->learnSpell(46584, false);
				player->learnSpell(49938, false);
				player->learnSpell(48707, false);
				player->learnSpell(48265, false);
				player->learnSpell(61999, false);
				player->learnSpell(42650, false);
				player->learnSpell(53428, false);
				player->learnSpell(53331, false);
				player->learnSpell(54447, false);
				player->learnSpell(53342, false);
				player->learnSpell(54446, false);
				player->learnSpell(53323, false);
				player->learnSpell(53344, false);
				player->learnSpell(70164, false);
				player->learnSpell(62158, false);
				player->learnSpell(33391, false);
				player->learnSpell(48778, false);
				player->learnSpell(51425, false);
				player->learnSpell(49924, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(55050)) // Heart Strike.
					player->learnSpell(55262, false);
				if (player->HasSpell(49143)) // Frost Strike.
					player->learnSpell(55268, false);
				if (player->HasSpell(49184)) // Howling Blast.
					player->learnSpell(51411, false);
				if (player->HasSpell(55090)) // Scourge Strike.
					player->learnSpell(55271, false);
				if (player->HasSpell(49158)) // Corpse Explosion.
					player->learnSpell(51328, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_SHAMAN:
				player->learnSpell(2062, false);
				player->learnSpell(8737, false);
				player->learnSpell(49273, false);
				player->learnSpell(49238, false);
				player->learnSpell(10399, false);
				player->learnSpell(49231, false);
				player->learnSpell(58753, false);
				player->learnSpell(2484, false);
				player->learnSpell(49281, false);
				player->learnSpell(58582, false);
				player->learnSpell(49233, false);
				player->learnSpell(58790, false);
				player->learnSpell(58704, false);
				player->learnSpell(58643, false);
				player->learnSpell(49277, false);
				player->learnSpell(61657, false);
				player->learnSpell(8012, false);
				player->learnSpell(526, false);
				player->learnSpell(2645, false);
				player->learnSpell(57994, false);
				player->learnSpell(8143, false);
				player->learnSpell(49236, false);
				player->learnSpell(58796, false);
				player->learnSpell(58757, false);
				player->learnSpell(49276, false);
				player->learnSpell(57960, false);
				player->learnSpell(131, false);
				player->learnSpell(58745, false);
				player->learnSpell(6196, false);
				player->learnSpell(58734, false);
				player->learnSpell(58774, false);
				player->learnSpell(58739, false);
				player->learnSpell(58656, false);
				player->learnSpell(546, false);
				player->learnSpell(556, false);
				player->learnSpell(66842, false);
				player->learnSpell(51994, false);
				player->learnSpell(8177, false);
				player->learnSpell(58749, false);
				player->learnSpell(20608, false);
				player->learnSpell(36936, false);
				player->learnSpell(36936, false);
				player->learnSpell(58804, false);
				player->learnSpell(49271, false);
				player->learnSpell(8512, false);
				player->learnSpell(6495, false);
				player->learnSpell(8170, false);
				player->learnSpell(66843, false);
				player->learnSpell(55459, false);
				player->learnSpell(66844, false);
				player->learnSpell(3738, false);
				player->learnSpell(2894, false);
				player->learnSpell(60043, false);
				player->learnSpell(51514, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->GetTeam() == ALLIANCE)
					player->learnSpell(32182, false); // Heroism.
				if (player->GetTeam() == HORDE)
					player->learnSpell(2825, false); // Bloodlust.
				if (player->HasSpell(61295)) // Riptide.
					player->learnSpell(61301, false);
				if (player->HasSpell(974)) // Earth Shield.
					player->learnSpell(49284, false);
				if (player->HasSpell(30706)) // Totem of Wrath.
					player->learnSpell(57722, false);
				if (player->HasSpell(51490)) // Thunderstorm.
					player->learnSpell(59159, false);
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case CLASS_DRUID:
				player->learnSpell(48378, false);
				player->learnSpell(48469, false);
				player->learnSpell(48461, false);
				player->learnSpell(48463, false);
				player->learnSpell(48441, false);
				player->learnSpell(53307, false);
				player->learnSpell(53308, false);
				player->learnSpell(5487, false);
				player->learnSpell(48560, false);
				player->learnSpell(6795, false);
				player->learnSpell(48480, false);
				player->learnSpell(53312, false);
				player->learnSpell(18960, false);
				player->learnSpell(5229, false);
				player->learnSpell(48443, false);
				player->learnSpell(50763, false);
				player->learnSpell(8983, false);
				player->learnSpell(8946, false);
				player->learnSpell(1066, false);
				player->learnSpell(48562, false);
				player->learnSpell(783, false);
				player->learnSpell(770, false);
				player->learnSpell(16857, false);
				player->learnSpell(18658, false);
				player->learnSpell(768, false);
				player->learnSpell(1082, false);
				player->learnSpell(16979, false);
				player->learnSpell(49376, false);
				player->learnSpell(5215, false);
				player->learnSpell(48477, false);
				player->learnSpell(49800, false);
				player->learnSpell(48465, false);
				player->learnSpell(48572, false);
				player->learnSpell(26995, false);
				player->learnSpell(48574, false);
				player->learnSpell(2782, false);
				player->learnSpell(50213, false);
				player->learnSpell(2893, false);
				player->learnSpell(33357, false);
				player->learnSpell(5209, false);
				player->learnSpell(48575, false);
				player->learnSpell(48447, false);
				player->learnSpell(48577, false);
				player->learnSpell(48579, false);
				player->learnSpell(5225, false);
				player->learnSpell(22842, false);
				player->learnSpell(49803, false);
				player->learnSpell(9634, false);
				player->learnSpell(20719, false);
				player->learnSpell(48467, false);
				player->learnSpell(29166, false);
				player->learnSpell(62600, false);
				player->learnSpell(22812, false);
				player->learnSpell(48470, false);
				player->learnSpell(48564, false);
				player->learnSpell(48566, false);
				player->learnSpell(33943, false);
				player->learnSpell(49802, false);
				player->learnSpell(48451, false);
				player->learnSpell(48568, false);
				player->learnSpell(33786, false);
				player->learnSpell(40120, false);
				player->learnSpell(62078, false);
				player->learnSpell(52610, false);
				player->learnSpell(50464, false);
				player->learnSpell(48570, false);
				player->learnSpell(34093, false);
				player->learnSpell(33392, false);
				if (player->HasSpell(50516)) // Typhoon.
					player->learnSpell(61384, false);
				if (player->HasSpell(48505)) // Starfall.
					player->learnSpell(53201, false);
				if (player->HasSpell(48438)) // Wild Growth.
					player->learnSpell(53251, false);
				if (player->HasSpell(5570)) // Insect Swarm.
					player->learnSpell(48468, false);
				player->SaveToDB();

		}


		// UPDATE A LA PREMIERE CONNEXION FIRST CONNEXION
		CharacterDatabase.PQuery("UPDATE characters SET date_data = CURRENT_DATE WHERE guid = %u", player->GetGUID());
		CharacterDatabase.PQuery("UPDATE characters SET date_end = CURRENT_DATE + INTERVAL 5  DAY WHERE guid = %u", player->GetGUID());
		CharacterDatabase.PQuery("UPDATE characters SET date_day = CURRENT_DATE WHERE guid = %u", player->GetGUID());
		CharacterDatabase.PQuery("INSERT INTO house_player (guid_player, exp, lvl, PA, PS, ENDU) VALUES (%u, 0, 0, 0 ,0, 0) ", player->GetGUID());
		CharacterDatabase.PQuery("INSERT INTO zone_player (guid) VALUES (%u) ", player->GetGUID());
		CharacterDatabase.PQuery("INSERT INTO boutique (guid) VALUES (%u) ", player->GetGUID());
		WorldDatabase.PQuery("INSERT INTO event_custom_player (guid, points) VALUES (%u, 0)", player->GetGUID());
		CharacterDatabase.PExecute("INSERT INTO ffarankings VALUES(%u, 0, 0, 0)", player->GetGUID());


		Quest const* quest = sObjectMgr->GetQuestTemplate(600613);
		Object * giver = player->ToCreature();
		player->AddQuest(quest, giver);
}

		if (player->GetTotalPlayedTime() < 1) {

		}

		if (player->GetTotalPlayedTime() > 2) {

			player->ApplyRatingMod(CR_CRIT_TAKEN_MELEE, 500, true);
			player->ApplyRatingMod(CR_CRIT_TAKEN_RANGED, 500, true);
			player->ApplyRatingMod(CR_CRIT_TAKEN_SPELL, 500, true);

			QueryResult result_carac = CharacterDatabase.PQuery("SELECT guid_player, PA, PS, ENDU FROM house_player WHERE guid_player = %u", player->GetGUID());

			Field *fields = result_carac->Fetch();
			uint32 guid_player = fields[0].GetUInt32();
			uint32 PA = fields[1].GetUInt32();
			uint32 PS = fields[2].GetUInt32();
			uint32 ENDU = fields[3].GetUInt32();

			for (uint32 i = 0; i < ENDU; ++i) {

				player->SetMaxHealth(player->GetMaxHealth() * (1 + 0.02));
			}

			for (uint32 i = 0; i < PS; ++i) {

				player->ApplySpellPowerBonus(player->GetBaseSpellPowerBonus() * (0.02), true);

			}

			for (uint32 i = 0; i < PA; ++i) {

				player->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, player->GetTotalAttackPowerValue(BASE_ATTACK) * (0.02), true);

				if (player->getClass() == CLASS_HUNTER) {

					player->HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, player->GetTotalAttackPowerValue(BASE_ATTACK) * (0.02), true);

				}

			}
		}

		

	}

};
class Item_construct : public ItemScript
{
public:
	Item_construct() : ItemScript("Item_construct") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/) override
	{

		player->PlayerTalkClass->ClearMenus();                              // Clears old options
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "", GOSSIP_SENDER_MAIN, 1);
		player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
		return true;

	}

	void OnGossipSelect(Player *player, Item* item, uint32 sender, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (uiAction)
		{
		case 1:
			if (GameObject * object = player->FindNearestGameObject(181075, 1))
			{
				if (object->isSpawned()){
					object->RemoveFromWorld();
					object->DeleteFromDB();
					object->Delete();
					player->RemoveGameObject(object, true);
					object->DeleteFromDB();
					object->Delete();
					player->RemoveGameObject(object, true);
					object->DeleteFromDB();
					player->AddItem(250017, 1);
				}
			}

			if (GameObject * object_1 = player->FindNearestGameObject(180334, 1)) {
				if (object_1->isSpawned()) {
					object_1->RemoveFromWorld();

					object_1->DeleteFromDB();
					object_1->Delete();
					player->RemoveGameObject(object_1, true);
					object_1->DeleteFromDB();
					object_1->Delete();
					player->RemoveGameObject(object_1, true);
					object_1->DeleteFromDB();
					player->AddItem(250018, 1);
				}

			}

			if (GameObject * object_2 = player->FindNearestGameObject(195063, 1)) {
				
				if (object_2->isSpawned()) {

					object_2->RemoveFromWorld();
					object_2->DeleteFromDB(); // not save respawn time
					object_2->Delete();
					player->RemoveGameObject(object_2, true);
					object_2->DeleteFromDB(); // not save respawn time
					object_2->Delete();
					player->RemoveGameObject(object_2, true);
					player->AddItem(250019, 1);

				}

			}

			if (GameObject * object_3 = player->FindNearestGameObject(193840, 1)) {
				
				if (object_3->isSpawned()) {
					object_3->RemoveFromWorld();

					object_3->DeleteFromDB(); // not save respawn time
					object_3->Delete();
					player->RemoveGameObject(object_3, true);
					object_3->DeleteFromDB(); // not save respawn time
					object_3->Delete();
					player->RemoveGameObject(object_3, true);

					player->AddItem(250020, 1);
				}
			}

			if (GameObject * object_4 = player->FindNearestGameObject(174849, 1)) {
				
				if (object_4->isSpawned()) {
					object_4->RemoveFromWorld();

					object_4->DeleteFromDB();// not save respawn time
					object_4->Delete();
					player->RemoveGameObject(object_4, true);
					object_4->DeleteFromDB();// not save respawn time
					object_4->Delete();
					player->RemoveGameObject(object_4, true);
					player->AddItem(250021, 1);
				}
			}

			if (GameObject * object_5 = player->FindNearestGameObject(180885, 1)) {
				
				if (object_5->isSpawned()) {
					
					object_5->RemoveFromWorld();
					object_5->DeleteFromDB();
					// not save respawn time
					object_5->Delete();
					player->RemoveGameObject(object_5, true);
					object_5->DeleteFromDB();
					// not save respawn time
					object_5->Delete();
					player->RemoveGameObject(object_5, true);
					player->AddItem(250022, 1);
				}
			}

			if (GameObject * object_6 = player->FindNearestGameObject(148914, 1)) {

				if (object_6->isSpawned()) {

					object_6->RemoveFromWorld();
					object_6->DeleteFromDB();
					// not save respawn time
					object_6->Delete();
					player->RemoveGameObject(object_6, true);
					object_6->DeleteFromDB();
					// not save respawn time
					object_6->Delete();
					player->RemoveGameObject(object_6, true);
					player->AddItem(250023, 1);
				}
			}

			if (GameObject * object_7 = player->FindNearestGameObject(333, 1)) {

				if (object_7->isSpawned()) {

					object_7->RemoveFromWorld();
					object_7->DeleteFromDB();
					// not save respawn time
					object_7->Delete();
					player->RemoveGameObject(object_7, true);
					object_7->DeleteFromDB();
					// not save respawn time
					object_7->Delete();
					player->RemoveGameObject(object_7, true);
					player->AddItem(250024, 1);
				}
			}

			if (GameObject * object_8 = player->FindNearestGameObject(183602, 1)) {

				if (object_8->isSpawned()) {

					object_8->RemoveFromWorld();
					object_8->DeleteFromDB();
					// not save respawn time
					object_8->Delete();
					player->RemoveGameObject(object_8, true);
					object_8->DeleteFromDB();
					// not save respawn time
					object_8->Delete();
					player->RemoveGameObject(object_8, true);
					player->AddItem(250025, 1);
				}
			}
			if (GameObject * object_9 = player->FindNearestGameObject(1760, 1)) {

				if (object_9->isSpawned()) {

					object_9->RemoveFromWorld();
					object_9->DeleteFromDB();
					// not save respawn time
					object_9->Delete();
					player->RemoveGameObject(object_9, true);
					object_9->DeleteFromDB();
					// not save respawn time
					object_9->Delete();
					player->RemoveGameObject(object_9, true);
					player->AddItem(250026, 1);
				}
			}
			if (GameObject * object_10 = player->FindNearestGameObject(181082, 1)) {

				if (object_10->isSpawned()) {

					object_10->RemoveFromWorld();
					object_10->DeleteFromDB();
					// not save respawn time
					object_10->Delete();
					player->RemoveGameObject(object_10, true);
					object_10->DeleteFromDB();
					// not save respawn time
					object_10->Delete();
					player->RemoveGameObject(object_10, true);
					player->AddItem(250027, 1);
				}
			}
			break;
		}
	}


};
class up_stat_mobs_1 : public CreatureScript // Enter zone
{

public:

	up_stat_mobs_1() : CreatureScript("up_stat_mobs_1") { }

	struct up_stat_mobs_1AI : public ScriptedAI
	{

		up_stat_mobs_1AI(Creature* creature) : ScriptedAI(creature) { }



		void EnterCombat(Unit* /*who*/) override
		{
			Player* target = sObjectAccessor->FindPlayer(me->GetTarget());
			if (target)
			{
				me->SetMaxHealth(target->GetMaxHealth());
				me->SetHealth(me->GetMaxHealth());
			}

		}

	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new up_stat_mobs_1AI(creature);
	}

};
class up_stat_mobs : public CreatureScript // Enter zone
{

public:

	up_stat_mobs() : CreatureScript("up_stat_mobs") { }

	struct up_stat_mobsAI : public ScriptedAI
	{

		up_stat_mobsAI(Creature* creature) : ScriptedAI(creature) { }		
		
		void EnterCombat(Unit* /*who*/) override
		{

			Player* target = sObjectAccessor->FindPlayer(me->GetTarget());
			if (target)
			{
				me->SetMaxHealth(target->GetMaxHealth());
				me->SetHealth(me->GetMaxHealth());
			}

			
		}



	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new up_stat_mobsAI(creature);
	}



};



class DrakeNPC : public CreatureScript // Enter zone
{

public:

	DrakeNPC() : CreatureScript("DrakeNPC") { }

	struct DrakeNPCAI : public ScriptedAI
	{

		DrakeNPCAI(Creature* creature) : ScriptedAI(creature) { 


		}
		void JustRespawned() {

			me->AddAura(50296, me);
			me->SetHover(true);
			me->SetCanFly(true);
			me->SetSpeed(MOVE_FLIGHT, 1.0f, true);

		}

		void EnterCombat(Unit* /*who*/) override
		{

		}

		void UpdateAI(uint32 diff) override
		{

			me->AddAura(50296, me);
			me->SetHover(true);
			me->SetCanFly(true);
			me->SetSpeed(MOVE_FLIGHT, 1.0f, true);
				
		}

	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new DrakeNPCAI(creature);
	}



};


class Civil_NPC : public CreatureScript
{

public:
	Civil_NPC() : CreatureScript("Civil_NPC") {}

	struct Civil_NPCAI : public ScriptedAI
	{

		Civil_NPCAI(Creature* creature) : ScriptedAI(creature) 
		{

			creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			creature->SetReactState(REACT_PASSIVE);

		}
		void JustDied(Unit* /*killer*/) override
		{
			me->setFaction(15);
		}
		void EnterCombat(Unit* /*who*/) override
		{
			Player* target = sObjectAccessor->FindPlayer(me->GetTarget());
			if (!target)
				return;
			
			me->SetMaxHealth(target->GetMaxHealth());
			me->SetHealth(me->GetMaxHealth());
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->setFaction(16);

		}

		void UpdateAI(uint32 diff) override
		{

			me->HandleEmoteCommand(1);


		}


	};

		bool OnGossipHello(Player *player, Creature *creature)
		{



			if (player->GetQuestStatus(900913) == QUEST_STATUS_INCOMPLETE) {
				player->ADD_GOSSIP_ITEM(0, "Est-ce que vous avez entendu parler d'un traitre ?", GOSSIP_SENDER_MAIN, 2);
				player->SEND_GOSSIP_MENU(40000, creature->GetGUID());


			}
			if (player->GetQuestStatus(200213) == QUEST_STATUS_INCOMPLETE) {
					player->ADD_GOSSIP_ITEM(0, "Est-ce que ca vous dirait de rentrer dans l'armee ?", GOSSIP_SENDER_MAIN, 1);
					player->SEND_GOSSIP_MENU(40000, creature->GetGUID());


			}
			if (player->HasItemCount(304260, 1)) {
				player->ADD_GOSSIP_ITEM(0, "Essayer de voler le civil", GOSSIP_SENDER_MAIN, 3);
				player->SEND_GOSSIP_MENU(40000, creature->GetGUID());


			}
				player->SEND_GOSSIP_MENU(40000, creature->GetGUID());
				return true;

		}
		bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
		{

			if (sender == GOSSIP_SENDER_MAIN)
			{
				switch (uiAction)
				{
				case 1:
					player->KilledMonsterCredit(253171);
					switch (urand(2, 5)) {

					case 2:
						creature->MonsterWhisper("He ho qu'est-ce que tu crois toi ?", player, false);
						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;
					case 3:
						creature->MonsterWhisper("Fichez-moi la paix !", player, false);
						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;
					case 4:
						creature->MonsterWhisper("Laissez moi tranquille !", player, false);
						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;
					case 5:
						creature->MonsterWhisper("Qu'est-ce que vous voulez ?", player, false);
						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;

					}



					break;

				case 2:
					switch (urand(2, 9)) {
					case 2:
						creature->MonsterWhisper("Non desole, je ne saurais pas vous aider", player, false);
						creature->DespawnOrUnsummon();

						player->CLOSE_GOSSIP_MENU();
						break;
					case 3:
						creature->MonsterWhisper("Degage !", player, false);
						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;
					case 4:
						creature->MonsterWhisper("Hum, qu'est-ce que c'est un traitre exactement ?", player, false);
						creature->DespawnOrUnsummon();

						player->CLOSE_GOSSIP_MENU();
						break;
					case 5:
						creature->MonsterWhisper("Qu'est-ce que vous voulez ? Je n'ai pas de temps a perdre moi !", player, false);
						creature->DespawnOrUnsummon();

						player->CLOSE_GOSSIP_MENU();
						break;
					case 6:
						creature->MonsterWhisper("Quoi ? Qu'est-ce que vous voulez ?", player, false);
						creature->DespawnOrUnsummon();

						player->CLOSE_GOSSIP_MENU();
						break;
					case 7:
						creature->MonsterWhisper("Non, mais si je le trouve je vous previendrai", player, false);
						creature->DespawnOrUnsummon();

						player->CLOSE_GOSSIP_MENU();
						break;
					case 8:
						creature->MonsterWhisper("Pas moyen d'etre tranquille dans cette ville de ... !", player, false);

						creature->AI()->AttackStart(player);
						player->CLOSE_GOSSIP_MENU();
						break;
					case 9:
						player->CastSpell(player, 71495);
						creature->MonsterWhisper("Comment avez vous fait ? Impossible ! Peut importe, je m'en vais vous reduire en poussiere !", player, false);
						creature->SummonCreature(355516, creature->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 300000);
						creature->DespawnOrUnsummon();
						player->CLOSE_GOSSIP_MENU();
						break;

					}
					break;
				case 3:
					switch (urand(0, 1)) {
					case 0:
						player->AddItem(60007, 1);
						creature->DespawnOrUnsummon();
						player->CLOSE_GOSSIP_MENU();

						break;
					case 1:
						creature->DespawnOrUnsummon();
						creature->MonsterWhisper("Vous n'avez pas reussi !", player, false);
						player->CLOSE_GOSSIP_MENU();

						break;
					}
						break;


				}
			}
			return true;
		}

		CreatureAI* GetAI(Creature* creature) const override
		{
			return new Civil_NPCAI(creature);
		}
};

class On_Change_Talent : public PlayerScript
{
public:
	On_Change_Talent() : PlayerScript("On_Change_Talent") {}
	
	void OnTalentsReset(Player * player, bool noCost) { 
	
		QueryResult result = CharacterDatabase.PQuery("SELECT max_talent FROM characters WHERE guid = %u", player->GetGUID());
		Field *fields = result->Fetch();
		uint32 max_talent = fields[0].GetUInt32();
		CharacterDatabase.PQuery("UPDATE characters SET max_talent = 0 WHERE guid = %u", player->GetGUID());
		player->AddItem(430405, max_talent * 40);
	
	}
};
class livre_gob2 : public GameObjectScript {


public:
	livre_gob2() : GameObjectScript("livre_gob2"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Prendre", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(62831, 1, true)) {
					player->CastSpell(player, 35426);
					player->SummonCreature(900015, go->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 20000);

				}
			}

		}

		return true;
	}

};
class planteGOB : public GameObjectScript {


public:
	planteGOB() : GameObjectScript("planteGOB"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Prendre", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(332022, 1, true)) {
					player->AddItem(373502, 2);
					player->CLOSE_GOSSIP_MENU();
					go->RemoveFromWorld();

				}
				else
				{
					
					player->AddItem(373502, 1);
					player->CLOSE_GOSSIP_MENU();
					go->RemoveFromWorld();


				}
			}

		}

		return true;
	}

};
class Bonus_game_time : public CreatureScript {


public:
	Bonus_game_time() : CreatureScript("Bonus_game_time"){}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		double game_time = (player->GetTotalPlayedTime() / 60 / 60);
		std::stringstream info_game_time;
		info_game_time << "Heure(s) de temps de jeu actuelle : " << game_time << "heure(s)" <<std::endl;
		player->ADD_GOSSIP_ITEM(0, info_game_time.str(), GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/spell_holy_borrowedtime:20|t|cffDF0101 1 Heure de temps de jeu (Bonus : 1 Marques Elegon World)|r", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/spell_holy_borrowedtime:20|t|cffDF0101 5 Heures de temps de jeu (Bonus : 5 Marques Elegon World)|r", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/spell_holy_borrowedtime:20|t|cffDF0101 10 Heures de temps de jeu (Bonus : 10 Marques Elegon World)|r", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/spell_holy_borrowedtime:20|t|cffDF0101 24 Heure de temps de jeu (Bonus : 15 Marques Elegon World)|r", GOSSIP_SENDER_MAIN, 4);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 uiAction)
	{
		double game_time = (player->GetTotalPlayedTime() / 60 / 60);
		QueryResult result = CharacterDatabase.PQuery("SELECT quest_complete FROM characters WHERE guid = %u", player->GetGUID());
		Field *fields = result->Fetch();
		uint32 quest_complete = fields[0].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (quest_complete <= 0 && game_time >= 1.0)
				{
					player->AddItem(456500, 1);
					CharacterDatabase.PQuery("UPDATE characters SET quest_complete = quest_complete + 1 WHERE guid = %u", player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de temps de jeu. Ou vous avez deja pris cette recompenses !");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 2:
				if (quest_complete <= 1 && game_time >= 5.0)
				{
					player->AddItem(456500, 5);
					CharacterDatabase.PQuery("UPDATE characters SET quest_complete = quest_complete + 1 WHERE guid = %u", player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de temps de jeu. Ou vous avez deja pris cette recompenses !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			case 3:
				if (quest_complete <= 2 && game_time >= 10.0)
				{
					player->AddItem(456500, 10);
					CharacterDatabase.PQuery("UPDATE characters SET quest_complete = quest_complete + 1 WHERE guid = %u", player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de temps de jeu. Ou vous avez deja pris cette recompenses !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			case 4:
				if (quest_complete <= 3 && game_time >= 24.0)
				{
					player->AddItem(456500, 15);
					CharacterDatabase.PQuery("UPDATE characters SET quest_complete = quest_complete + 1 WHERE guid = %u", player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de temps de jeu. Ou vous avez deja pris cette recompenses !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			case 5:
				if (quest_complete <= 4 && game_time >= 36.0)
				{
					player->AddItem(456500, 20);
					CharacterDatabase.PQuery("UPDATE characters SET quest_complete = quest_complete + 1 WHERE guid = %u", player->GetGUID());
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de temps de jeu. Ou vous avez deja pris cette recompenses !");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			}
		}
		return true;
	}
};
class tonneauGOB : public GameObjectScript {


public:
	tonneauGOB() : GameObjectScript("tonneauGOB"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Prendre", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(373502, 10, true)) {
					switch (urand(1, 5))
					{
					case 1:
						player->AddItem(250012, urand(1, 10));
						break;
					case 2:
						player->AddItem(430407, 1);
						break;
					case 3:
						player->SetHonorPoints(player->GetHonorPoints() + urand(1, 500));
						break;
					case 4:
						player->ModifyMoney(urand(1000, 500000));
						break;
					case 5:
						player->AddItem(urand(250000, 250002), 1);
						break;
					}
					player->DestroyItemCount(373502, 10, true);
					player->CLOSE_GOSSIP_MENU();
					player->AddItem(373503, 1);
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de plantes incurvees de malte...");
					player->CLOSE_GOSSIP_MENU();
				}
			}

		}

		return true;
	}

};
class Quest_complet_script : public CreatureScript { // Toutes les quêtes completée


public:
	Quest_complet_script() : CreatureScript("Quest_complet_script"){}

	bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/)
	{
		if (quest->GetQuestId() == 900950)
		{
			switch (urand(1, 3))
			{
			case 1:
				player->ModifyMoney(player->GetMoney() + 25000000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les pieces d'or !");
				player->AddAura(68852, player);
				break;
			case 2:
				player->SetHonorPoints(player->GetHonorPoints() + 10000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les points d'honneur !");
				player->AddAura(68852, player);

				break;
			case 3:
				player->AddItem(250003, 5);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les tokens equipements !");
				player->AddAura(68852, player);

				break;
			}
		}
		if (quest->GetQuestId() == 900951)
		{
			switch (urand(1, 3))
			{
			case 1:
				player->ModifyMoney(player->GetMoney() + 25000000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les pieces d'or !");
				player->AddAura(68852, player);

				break;
			case 2:
				player->SetHonorPoints(player->GetHonorPoints() + 10000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les points d'honneur !");
				player->AddAura(68852, player);

				break;
			case 3:
				player->AddItem(250004, 2);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les tokens armes !");
				player->AddAura(68852, player);

				break;
			}
		}
		if (quest->GetQuestId() == 900952)
		{
			switch (urand(1, 3))
			{
			case 1:
				player->ModifyMoney(player->GetMoney() + 25000000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les pieces d'or !");
				player->AddAura(68852, player);

				break;
			case 2:
				player->SetHonorPoints(player->GetHonorPoints() + 10000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les points d'honneur !");
				player->AddAura(68852, player);

				break;
			case 3:
				player->AddItem(250005, 1);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les tokens armes !");
				player->AddAura(68852, player);

				break;
			}
		}
		if (quest->GetQuestId() == 900953)
		{
			switch (urand(1, 2))
			{
			case 1:
				player->ModifyMoney(player->GetMoney() + 100000000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les pieces d'or !");
				player->AddAura(68852, player);

				break;
			case 2:
				player->SetHonorPoints(player->GetHonorPoints() + 25000);
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagne les points d'honneur !");
				player->AddAura(68852, player);

				break;
			}
		}
		return true;

	}
};
class RockGob : public GameObjectScript {


public:
	RockGob() : GameObjectScript("RockGob"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{

		player->ADD_GOSSIP_ITEM(2, "Prendre", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1: {
				if (go->GetEntry() == 1615270)
				{
					if (player->HasItemCount(332030, 1)) // Item bombe atomique
					{
						switch (urand(1, 5))
						{
						case 1:
							break;
						case 2:
							player->AddItem(234400, 1);
							break;
						case 3:
							break;
						}
						go->RemoveFromWorld();
						go->Delete();
						player->AddItem(283502, 10);
						player->CLOSE_GOSSIP_MENU();
						return false;
					}
					if (player->HasItemCount(33229, 1)) // Item Explosifs NG-05
					{
						switch (urand(1, 5))
						{
						case 1:
							break;
						case 2:
							player->AddItem(234400, 1);
							break;
						case 3:
							break;
						}
						go->RemoveFromWorld();
						go->Delete();
						player->AddItem(283502, 3);
						player->CLOSE_GOSSIP_MENU();
						return false;
					}
					if (player->HasItemCount(33228, 1)) // Item bombe atomique
					{
						switch (urand(1, 5))

			case 1:
				break;
			case 2:
				player->AddItem(234400, 1);
				break;
			case 3:
				break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283502, 2);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(33227, 1)) // Item bombe atomique
				{
					switch (urand(1, 5))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283502, 1);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les objets requis pour recolter ce genre de roche");
					player->CLOSE_GOSSIP_MENU();
				}
			}

			if (go->GetEntry() == 1615269)
			{
				if (player->HasItemCount(332030, 1)) // Item bombe atomique
				{

					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283501, 89);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332029, 1)) // Item Explosifs NG-05
				{
					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283501, 46);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332028, 1)) // Item bombe atomique
				{
					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283501, 20);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332027, 1)) // Item bombe atomique
				{
					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283501, 10);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332026, 1)) // Item bombe atomique
				{
					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283501, 5);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332025, 1)) // Item bombe atomique
				{
					switch (urand(1, 7))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					}
					go->RemoveFromWorld();
					player->AddItem(283501, 2);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332024, 1)) // Item bombe atomique
				{
					go->IsInvisibleDueToDespawn();
					player->AddItem(283501, 1);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas les objets requis pour recolter ce genre de roche");
					player->CLOSE_GOSSIP_MENU();
				}
			}
			if (go->GetEntry() == 1615268)
			{
				if (player->HasItemCount(332030, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 192);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332029, 1)) // Item Explosifs NG-05
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 86);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332028, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 40);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332027, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 20);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332026, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 10);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332025, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 7);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332024, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 5);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
				if (player->HasItemCount(332023, 1)) // Item bombe atomique
				{
					switch (urand(1, 10))
					{
					case 1:
						break;
					case 2:
						player->AddItem(234400, 1);
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					}
					go->RemoveFromWorld();
					go->RemoveFromWorld();
					go->Delete();
					player->AddItem(283500, 1);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
			}
				
				break;

			}

		}
		return true;
	}

};
class Prospect_NPC : public CreatureScript {


public:
	Prospect_NPC() : CreatureScript("Prospect_NPC"){}

	bool OnGossipHello(Player* player, Creature* creature)
	{

		player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|t Prospecter mes Roches simples", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|t Prospecter mes Roches rares", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|t Prospecter mes Roche adamantite", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Qu'est-ce que vous avez me vendre ?", GOSSIP_SENDER_MAIN, 4);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (action)
			{
			case 1: {
						int count = player->GetItemCount(283500); // Nombre de pierre simple.
						for (int i(0); i < count; i++) // Passage dans une boucle pour repeter autant de fois qu'il a de pierre simple.
						{
							switch (urand(1, 100))
							{
							case 1:
								player->AddItem(234401, urand(1, 10));
								break;
							}
						}
						player->DestroyItemCount(283500, count, true);
						player->CLOSE_GOSSIP_MENU();
			}
				break; 
				

				
				
			case 2: {int count_i = player->GetItemCount(283501); // Nombre de pierre simple.
				for (int i(0); i < count_i; i++) // Passage dans une boucle pour repeter autant de fois qu'il a de pierre simple.
				{
					switch (urand(1, 50))
					{
					case 1: 
						player->AddItem(234402, urand(1, 10));
						break;
					}
				}
				player->DestroyItemCount(283501, count_i, true);
				player->CLOSE_GOSSIP_MENU();


			}
				break;

			case 3:  {int count_j = player->GetItemCount(283502); // Nombre de pierre simple.
				for (int i(0); i < count_j; i++) // Passage dans une boucle pour repeter autant de fois qu'il a de pierre simple.
				{
					switch (urand(1, 25))
					{
					case 1: 
						player->AddItem(234403, urand(1, 10));
						break;

					}
				}
				player->DestroyItemCount(283502, count_j, true);
				player->CLOSE_GOSSIP_MENU();

			}
				break;
	
			case 4:
				player->GetSession()->SendListInventory(creature->GetGUID());
				break;

			}
		}
		return true;
	}

};
void AddSC_house_script()
{

	new On_Change_Talent();
	new Charbon_constructor();
	new On_Login_Housing();
	new NPC_House_blacksmith();
	new NPC_House_main();
	new NPC_House_taillor();
	new Spawn_quest_lutin();
	new Teleporter_GOB();
	new up_stat_mobs();
	new up_stat_mobs_1();
	new DrakeNPC();
	new Civil_NPC();
	new Item_construct();
	new NPC_Extension();
	new livre_gob2();
	new Teleporter_GOB2();
	new planteGOB();
	new tonneauGOB();
	new Quest_complet_script();
	new RockGob();
	new Prospect_NPC();
	new Bonus_game_time();
}
