#include "ScriptMgr.h"
#include "Group.h"
#include "Object.h"
#include "Common.h"
#include <ace/Singleton.h>
#include <ace/Atomic_Op.h>

#include "DBCStores.h"
#include "QuestDef.h"
#include "SharedDefines.h"
#include "World.h"
#include "Weather.h"

using namespace std;
#define COST_VEGATABLE 100000 
#define COST_VEGATABLE_BONUS 100000

#define BONUS_PA 446140
#define BONUS_PS 446141
#define BONUS_EN 446142
#define BONUS_HA 446143





class GameObject_Vegetable : public GameObjectScript {

private:




public:
	GameObject_Vegetable() : GameObjectScript("GameObject_Vegetable"){}


	bool OnGossipHello(Player* player, GameObject* go)
	{

		QueryResult result_data = CharacterDatabase.PQuery("SELECT guid, progression ,progression_1 ,progression_2 ,progression_3, vie ,vie_1 ,vie_2 ,vie_3 , in_progress, in_progress_1, in_progress_2, in_progress_3, buyslot , buyslot_1 , buyslot_2, buyslot_3, type, type_1, type_2, type_3 FROM zone_player WHERE guid = %u", player->GetGUID());
		Field *fields = result_data->Fetch();
		uint32 guid_player = fields[0].GetUInt32();

		/*Progression des plantes*/
		float progression = fields[1].GetFloat();
		float progression_1 = fields[2].GetFloat();
		float progression_2 = fields[3].GetFloat();
		float progression_3 = fields[4].GetFloat();
		/*Points de vie des plantes*/

		uint32 vie = fields[5].GetUInt32();
		uint32 vie_1 = fields[6].GetUInt32();
		uint32 vie_2 = fields[7].GetUInt32();
		uint32 vie_3 = fields[8].GetUInt32();

		/*Si la plante est en cours */
		uint32 in_progress = fields[9].GetUInt32();
		uint32 in_progress_1 = fields[10].GetUInt32();
		uint32 in_progress_2 = fields[11].GetUInt32();
		uint32 in_progress_3 = fields[12].GetUInt32();

		/*Si elles ont été acheté ou pas*/
		uint32 buyslot = fields[13].GetUInt32();
		uint32 buyslot_1 = fields[14].GetUInt32();
		uint32 buyslot_2 = fields[15].GetUInt32();
		uint32 buyslot_3 = fields[16].GetUInt32();

		/*Type de plante*/
		uint32 type = fields[17].GetUInt32();
		uint32 type_1 = fields[18].GetUInt32();
		uint32 type_2 = fields[19].GetUInt32();
		uint32 type_3 = fields[20].GetUInt32();


		/* if (buyslot == 0) {
		player->CLOSE_GOSSIP_MENU();
		player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas acheter de plantation");
		return true;
		} */



		if (buyslot == 1 && go->GetEntry() == 600002) {

			stringstream warning;
			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (1) : " << vie << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (1) : " << progression << " %" << std::endl;
			player->ADD_GOSSIP_ITEM(0, warning.str(), GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());


			if (in_progress == 0) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:20|t Planter une graine : Puissance d'attaque", GOSSIP_SENDER_MAIN, 2); // Inserer une graine PA
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_mage_arcanebarrage:20|t Planter une graine : Puissance des sorts", GOSSIP_SENDER_MAIN, 3); // Inserer une graine PS
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_racial_bearform:20|t Planter une graine : Endurance", GOSSIP_SENDER_MAIN, 4); // Inserer une graine Endurance
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_skinteeth:20|t Planter une graine : Hate", GOSSIP_SENDER_MAIN, 5); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());

			}
			else if (type == 1 && progression >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance d'attaque", GOSSIP_SENDER_MAIN, 50); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_1 == 1 && progression >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance des sort", GOSSIP_SENDER_MAIN, 51); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_2 == 1 && progression >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Endurance", GOSSIP_SENDER_MAIN, 52); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_3 == 1 && progression >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Hate", GOSSIP_SENDER_MAIN, 53); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (in_progress == 1) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_empoweredrejuvination:30|t Graine en cours", GOSSIP_SENDER_MAIN, 10); // Inserer une graine Hate
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_nourish:30|t Arroser la plante", GOSSIP_SENDER_MAIN, 100); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
				return true;
			}
		}


		if (buyslot_1 == 1 && go->GetEntry() == 600003) {
			stringstream warning;
			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (1) : " << vie_1 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (1) : " << progression_1 << " %" << std::endl;

			player->ADD_GOSSIP_ITEM(0, warning.str(), GOSSIP_SENDER_MAIN, 1);
			if (in_progress_1 == 0) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:20|t Planter une graine : Puissance d'attaque", GOSSIP_SENDER_MAIN, 2); // Inserer une graine PA
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_mage_arcanebarrage:20|t Planter une graine: Puissance des sorts", GOSSIP_SENDER_MAIN, 3); // Inserer une graine PS
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_racial_bearform:20|t Planter une graine : Endurance", GOSSIP_SENDER_MAIN, 4); // Inserer une graine Endurance
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_skinteeth:20|t Planter une graine: Hate", GOSSIP_SENDER_MAIN, 5); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());

			}
			else if (type == 1 && progression_1 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance d'attaque", GOSSIP_SENDER_MAIN, 60); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_1 == 1 && progression_1 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance des sorts", GOSSIP_SENDER_MAIN, 61); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_2 == 1 && progression_1 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Endurance", GOSSIP_SENDER_MAIN, 62); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_3 == 1 && progression_1 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Hate", GOSSIP_SENDER_MAIN, 63); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (in_progress_1 == 1) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_empoweredrejuvination:30|t Graine en cours", GOSSIP_SENDER_MAIN, 10); // Inserer une graine Hate
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_nourish:30|t Arroser la plante", GOSSIP_SENDER_MAIN, 100); // Inserer une graine Hate

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());

				return true;
			}
		}


		if (buyslot_2 == 1 && go->GetEntry() == 600004) {

			stringstream warning;
			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (1) : " << vie_2 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (1) : " << progression_2 << " %" << std::endl;

			player->ADD_GOSSIP_ITEM(0, warning.str(), GOSSIP_SENDER_MAIN, 1);
			if (in_progress_2 == 0) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:20|t Planter une graine : Puissance d'attaque", GOSSIP_SENDER_MAIN, 2); // Inserer une graine PA
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_mage_arcanebarrage:20|t Planter une graine : Puissance des sorts", GOSSIP_SENDER_MAIN, 3); // Inserer une graine PS
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_racial_bearform:20|t Planter une graine : Endurance", GOSSIP_SENDER_MAIN, 4); // Inserer une graine Endurance
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_skinteeth:20|t Planter une graine : Hate", GOSSIP_SENDER_MAIN, 5); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());

			}
			else if (type == 1 && progression_2 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance d'attaque", GOSSIP_SENDER_MAIN, 70); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_1 == 1 && progression_2 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_mage_arcanebarrage:30|t Recuperer : Graine Puissance des sorts", GOSSIP_SENDER_MAIN, 71); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_2 == 1 && progression_2 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Endurance", GOSSIP_SENDER_MAIN, 72); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_3 == 1 && progression_2 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Hate", GOSSIP_SENDER_MAIN, 73); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (in_progress_2 == 1) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_empoweredrejuvination:30|t Graine en cours", GOSSIP_SENDER_MAIN, 10); // Inserer une graine Hate
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_nourish:30|t Arroser la plante", GOSSIP_SENDER_MAIN, 100); // Inserer une graine Hate

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
				return true;
			}
		}



		if (buyslot_3 == 1 && go->GetEntry() == 600005) {

			stringstream warning;
			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (1) : " << vie_3 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (1) : " << progression_3 << " %" << std::endl;

			player->ADD_GOSSIP_ITEM(0, warning.str(), GOSSIP_SENDER_MAIN, 1);
			if (in_progress_3 == 0) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:20|t Planter une graine : Puissance d'attaque", GOSSIP_SENDER_MAIN, 2); // Inserer une graine PA
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_mage_arcanebarrage:20|tP lanter une graine : Puissance des sorts", GOSSIP_SENDER_MAIN, 3); // Inserer une graine PS
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_racial_bearform:20|t Planter une graine : Endurance", GOSSIP_SENDER_MAIN, 4); // Inserer une graine Endurance
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_skinteeth:20|t Planter une graine : Hate", GOSSIP_SENDER_MAIN, 5); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());


			}
			else if (type == 1 && progression_3 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance d'attaque", GOSSIP_SENDER_MAIN, 80); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_1 == 1 && progression_3 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Puissance des sorts", GOSSIP_SENDER_MAIN, 81); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_2 == 1 && progression_3 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Endurance", GOSSIP_SENDER_MAIN, 82); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (type_3 == 1 && progression_3 >= 100) {
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_rake:30|t Recuperer : Graine Hate", GOSSIP_SENDER_MAIN, 83); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else if (in_progress_3 == 1) {

				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_empoweredrejuvination:30|t Graine en cours", GOSSIP_SENDER_MAIN, 10); // Inserer une graine Hate
				player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_druid_nourish:30|t Arroser la plante", GOSSIP_SENDER_MAIN, 100); // Inserer une graine Hate
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
				return true;

			}
		}
		return true;
	}
	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
	{

		QueryResult result_data = CharacterDatabase.PQuery("SELECT guid, progression ,progression_1 ,progression_2 ,progression_3, vie ,vie_1 ,vie_2 ,vie_3 , in_progress, in_progress_1, in_progress_2, in_progress_3, buyslot , buyslot_1 , buyslot_2, buyslot_3, type, type_1, type_2, type_3 FROM zone_player WHERE guid = %u", player->GetGUID());
		Field *fields = result_data->Fetch();

		uint32 guid = fields[0].GetUInt32();

		/*Progression des plantes*/
		float progression = fields[1].GetFloat();
		float progression_1 = fields[2].GetFloat();
		float progression_2 = fields[3].GetFloat();
		float progression_3 = fields[4].GetFloat();
		/*Points de vie des plantes*/

		uint32 vie = fields[5].GetUInt32();
		uint32 vie_1 = fields[6].GetUInt32();
		uint32 vie_2 = fields[7].GetUInt32();
		uint32 vie_3 = fields[8].GetUInt32();

		/*Si la plante est en cours */
		uint32 in_progress = fields[9].GetUInt32();
		uint32 in_progress_1 = fields[10].GetUInt32();
		uint32 in_progress_2 = fields[11].GetUInt32();
		uint32 in_progress_3 = fields[12].GetUInt32();

		/*Si elles ont été acheté ou pas*/
		uint32 buyslot = fields[13].GetUInt32();
		uint32 buyslot_1 = fields[14].GetUInt32();
		uint32 buyslot_2 = fields[15].GetUInt32();
		uint32 buyslot_3 = fields[16].GetUInt32();

		/*Type de plante*/
		uint32 type = fields[17].GetUInt32();
		uint32 type_1 = fields[18].GetUInt32();
		uint32 type_2 = fields[19].GetUInt32();
		uint32 type_3 = fields[20].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (action)
			{
			case 100:
				if (go->GetEntry() == 600002) {
					player->GetSession()->SendAreaTriggerMessage("Votre plante a bien ete arrosee !");
					player->CLOSE_GOSSIP_MENU();

					CharacterDatabase.PQuery("UPDATE zone_player set vie = 100 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1
				}
				if (go->GetEntry() == 600003) {
					player->GetSession()->SendAreaTriggerMessage("Votre plante a bien ete arrosee !");
					player->CLOSE_GOSSIP_MENU();

					CharacterDatabase.PQuery("UPDATE zone_player set vie_1 = 100 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

				}
				if (go->GetEntry() == 600004) {
					player->GetSession()->SendAreaTriggerMessage("Votre plante a bien ete arrosee !");
					player->CLOSE_GOSSIP_MENU();

					CharacterDatabase.PQuery("UPDATE zone_player set vie_2 = 100 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

				}
				if (go->GetEntry() == 600005) {
					player->GetSession()->SendAreaTriggerMessage("Votre plante a bien ete arrosee !");
					player->CLOSE_GOSSIP_MENU();

					CharacterDatabase.PQuery("UPDATE zone_player set vie_3 = 100 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

				}
				break;
			case 2: // Planter une graine de PA
				if (player->HasItemCount(73901, 1, true)) {
					player->DestroyItemCount(73901, 1, true);
					CharacterDatabase.PQuery("UPDATE zone_player set type = type + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1
					if (go->GetEntry() == 600002) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress = in_progress + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600003) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = in_progress_1 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600004) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = in_progress_2 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600005) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = in_progress_3 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas la graine requise !");


				}
				break;
			case 3: // Planter une graine de PS
				if (player->HasItemCount(73900, 1, true)) {
					player->DestroyItemCount(73900, 1, true);
					CharacterDatabase.PQuery("UPDATE zone_player set type_1 = type_1 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1
					if (go->GetEntry() == 600002) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress = in_progress + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600003) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = in_progress_1 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600004) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = in_progress_2 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600005) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = in_progress_3 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas la graine requise !");


				}
				break;
			case 4: // Planter une graine de PA
				if (player->HasItemCount(73902, 1, true)) {
					player->DestroyItemCount(73902, 1, true);
					CharacterDatabase.PQuery("UPDATE zone_player set type_2 = type_2 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1
					if (go->GetEntry() == 600002) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress = in_progress + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600003) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = in_progress_1 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600004) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = in_progress_2 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600005) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = in_progress_3 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas la graine requise !");


				}
				break;
			case 5: // Planter une graine de HATE
				if (player->HasItemCount(73903, 1, true)) {
					player->DestroyItemCount(73903, 1, true);
					CharacterDatabase.PQuery("UPDATE zone_player set type_3 = type_3 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1
					if (go->GetEntry() == 600002) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress = in_progress + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600003) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = in_progress_1 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600004) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = in_progress_2 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					if (go->GetEntry() == 600005) {
						CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = in_progress_3 + 1 WHERE guid = %u ", player->GetGUID()); // Plante une graine de type en 1

					}
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas la graine requise !");


				}
				break;
			case 50: // Récompences in_progress PA
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress = in_progress - 1 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0

				break;
			case 60: // Récompences in_progress_1 PA
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_1 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0

				break;
			case 70: // Récompences in_progress_2 PA
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_2 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0

				break;
			case 80: // Récompences in_progress_3 PA
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = in_progress - 1 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_3 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				break;

			case 51: // Récompences in_progress PS
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PS, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				break;

			case 61: // Récompences in_progress_1 PS
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PS, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_1 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				break;

			case 71: // Récompences in_progress_2 PS
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PS, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_2 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				break;

			case 81: // Récompences in_progress_3 PS
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_PS, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_3 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				break;

			case 52: // Récompences in_progress ENDU
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_EN, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;
			case 62: // Récompences in_progress_1 ENDU
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_EN, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_1 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;
			case 72: // Récompences in_progress_2 ENDU
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_EN, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_2 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;
			case 82: // Récompences in_progress_3 ENDU
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_EN, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_3 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;

			case 53: // Récompences in_progress HATE
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_HA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;

			case 63: // Récompences in_progress_1 HATE
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_HA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_1 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_1 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;

			case 73: // Récompences in_progress_2 HATE
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_HA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_2 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;

			case 83: // Récompences in_progress_3 HATE
				player->CLOSE_GOSSIP_MENU();
				player->AddItem(BONUS_HA, 1);
				player->GetSession()->SendAreaTriggerMessage("Votre recompense est dans votre sac");
				CharacterDatabase.PQuery("UPDATE zone_player set in_progress_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le in_progress à 0
				CharacterDatabase.PQuery("UPDATE zone_player set vie_3 = 100 WHERE guid = %u ", player->GetGUID()); // Reset des points de vie à 0
				CharacterDatabase.PQuery("UPDATE zone_player set progression_3 = 0 WHERE guid = %u ", player->GetGUID()); // On reset la progression à 0
				CharacterDatabase.PQuery("UPDATE zone_player set type_2 = 0 WHERE guid = %u ", player->GetGUID()); // On reset le type à 0
				break;
			}
		}
		return true;
	}

};

class NPC_Vegetable : public CreatureScript {


public:
	NPC_Vegetable() : CreatureScript("NPC_Vegetable"){}

	bool OnGossipHello(Player* player, Creature* creature)
	{

		QueryResult result_data = CharacterDatabase.PQuery("SELECT guid, progression ,progression_1 ,progression_2 ,progression_3, vie ,vie_1 ,vie_2 ,vie_3 , in_progress, in_progress_1, in_progress_2, in_progress_3, buyslot , buyslot_1 , buyslot_2, buyslot_3, type, type_1, type_2, type_3 FROM zone_player WHERE guid = %u", player->GetGUID());
		Field *fields = result_data->Fetch();

		uint32 guid = fields[0].GetUInt32();

		/*Progression des plantes*/
		float progression = fields[1].GetFloat();
		float progression_1 = fields[2].GetFloat();
		float progression_2 = fields[3].GetFloat();
		float progression_3 = fields[4].GetFloat();
		/*Points de vie des plantes*/

		uint32 vie = fields[5].GetUInt32();
		uint32 vie_1 = fields[6].GetUInt32();
		uint32 vie_2 = fields[7].GetUInt32();
		uint32 vie_3 = fields[8].GetUInt32();

		/*Si la plante est en cours */
		uint32 in_progress = fields[9].GetUInt32();
		uint32 in_progress_1 = fields[10].GetUInt32();
		uint32 in_progress_2 = fields[11].GetUInt32();
		uint32 in_progress_3 = fields[12].GetUInt32();

		/*Si elles ont été acheté ou pas*/
		uint32 buyslot = fields[13].GetUInt32();
		uint32 buyslot_1 = fields[14].GetUInt32();
		uint32 buyslot_2 = fields[15].GetUInt32();
		uint32 buyslot_3 = fields[16].GetUInt32();

		/*Type de plante*/
		uint32 type = fields[17].GetUInt32();
		uint32 type_1 = fields[18].GetUInt32();
		uint32 type_2 = fields[19].GetUInt32();
		uint32 type_3 = fields[20].GetUInt32();


		if (buyslot == 0) {

			std::stringstream cost;
			cost << "Acheter une portion de terre [Prix : " << COST_VEGATABLE << " pieces d'or]";
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, cost.str(), GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Fermer", GOSSIP_SENDER_MAIN, 10);
			player->SEND_GOSSIP_MENU(26500, creature->GetGUID());
			return true;
		}
		stringstream warning;
		if (buyslot == 1) {

			warning << "|cffFF0000 * INFORMATIONS PLANTATIONS * |r" << std::endl << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (1) : " << vie << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (1) : " << progression << " %" << std::endl;

		}

		if (buyslot_1 == 1) {

			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie  (2) : " << vie_1 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (2) : " << progression_1 << " %" << std::endl;
		}

		if (buyslot_2 == 1) {

			warning << "|TInterface/ICONS/Ability_druid_empoweredtouch:20|t Points de vie (3) : " << vie_1 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (3) : " << progression_2 << " %" << std::endl;

		}

		if (buyslot_3 == 1) {

			warning << "|TInterface/ICONS/Ability_druid _empoweredtouch:20|t Points de vie (4) : " << vie_3 << " HP" << std::endl;
			warning << "|TInterface/ICONS/Ability_druid_flourish:20|t Progression (4) : " << progression_3 << " %" << std::endl;

		}

		player->ADD_GOSSIP_ITEM(0, warning.str(), GOSSIP_SENDER_MAIN, 30);

		std::stringstream cost_bonus;
		cost_bonus << " + 1 Portion [Prix : " << COST_VEGATABLE_BONUS << " pieces d'or]";
		if (buyslot_3 == 1) { // Si il a le maximal de plante alors on lui affiche rien
		}
		else {

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, cost_bonus.str(), GOSSIP_SENDER_MAIN, 2);
		}



		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Boutique", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Fermer", GOSSIP_SENDER_MAIN, 10);
		player->SEND_GOSSIP_MENU(26500, creature->GetGUID());
		return true;

	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 uiAction)
	{
		QueryResult result_data = CharacterDatabase.PQuery("SELECT guid, progression ,progression_1 ,progression_2 ,progression_3, vie ,vie_1 ,vie_2 ,vie_3 , in_progress, in_progress_1, in_progress_2, in_progress_3, buyslot , buyslot_1 , buyslot_2, buyslot_3, type, type_1, type_2, type_3 FROM zone_player WHERE guid = %u", player->GetGUID());
		Field *fields = result_data->Fetch();

		uint32 guid = fields[0].GetUInt32();

		/*Progression des plantes*/
		float progression = fields[1].GetFloat();
		float progression_1 = fields[2].GetFloat();
		float progression_2 = fields[3].GetFloat();
		float progression_3 = fields[4].GetFloat();
		/*Points de vie des plantes*/

		uint32 vie = fields[5].GetUInt32();
		uint32 vie_1 = fields[6].GetUInt32();
		uint32 vie_2 = fields[7].GetUInt32();
		uint32 vie_3 = fields[8].GetUInt32();

		/*Si la plante est en cours */
		uint32 in_progress = fields[9].GetUInt32();
		uint32 in_progress_1 = fields[10].GetUInt32();
		uint32 in_progress_2 = fields[11].GetUInt32();
		uint32 in_progress_3 = fields[12].GetUInt32();

		/*Si elles ont été acheté ou pas*/
		uint32 buyslot = fields[13].GetUInt32();
		uint32 buyslot_1 = fields[14].GetUInt32();
		uint32 buyslot_2 = fields[15].GetUInt32();
		uint32 buyslot_3 = fields[16].GetUInt32();

		/*Type de plante*/
		uint32 type = fields[17].GetUInt32();
		uint32 type_1 = fields[18].GetUInt32();
		uint32 type_2 = fields[19].GetUInt32();
		uint32 type_3 = fields[20].GetUInt32();

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->GetMoney() >= COST_VEGATABLE * 10000) {

					player->GetSession()->SendAreaTriggerMessage("Vous pouvez maintenant planter une graine dans la plantation adequate. Pour acheter une graine, parlez au fermier");
					CharacterDatabase.PQuery("UPDATE zone_player set buyslot = 1 WHERE guid = %u ", player->GetGUID()); // On l'insère dans la base de donnée.
					player->ModifyMoney(-COST_VEGATABLE * 10000);
					player->CLOSE_GOSSIP_MENU();

				}
				else {

					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez d'argent");

				}

				break;
			case 3:
				player->GetSession()->SendListInventory(creature->GetGUID());
				break;

			case 2: // Achat d'une plantation supplémentaire.

				if (player->GetMoney() >= COST_VEGATABLE_BONUS * 1000) {

					if (buyslot_1 == 0 && buyslot_2 == 0 && buyslot_3 == 0) { // Si la seconde plantation n'as pas été achetée
						player->GetSession()->SendAreaTriggerMessage("Vous venez d'acheter une premiere plantation");

						CharacterDatabase.PQuery("UPDATE zone_player set buyslot_1 = 1 WHERE guid = %u ", player->GetGUID()); // On l'insère dans la base de donnée.
						player->ModifyMoney(-COST_VEGATABLE_BONUS * 1000);

						player->CLOSE_GOSSIP_MENU();



					}

					if (buyslot_1 == 1 && buyslot_2 == 0 && buyslot_3 == 0)
					{
						CharacterDatabase.PQuery("UPDATE zone_player set buyslot_2 = 1 WHERE guid = %u ", player->GetGUID()); // On l'insère dans la base de donnée.
						player->GetSession()->SendAreaTriggerMessage("Vous venez d'acheter une seconde plantation");
						player->ModifyMoney(-COST_VEGATABLE_BONUS * 1000);

						player->CLOSE_GOSSIP_MENU();

					}

					if (buyslot_1 == 1 && buyslot_2 == 1 && buyslot_3 == 0)
					{
						CharacterDatabase.PQuery("UPDATE zone_player set buyslot_3 = 1 WHERE guid = %u ", player->GetGUID()); // On l'insère dans la base de donnée.
						player->GetSession()->SendAreaTriggerMessage("Vous venez d'acheter une troisieme plantation");
						player->ModifyMoney(-COST_VEGATABLE_BONUS * 1000);

						player->CLOSE_GOSSIP_MENU();


					}

					if (buyslot_1 == 1 && buyslot_2 == 1 && buyslot_3 == 1)
					{
						player->GetSession()->SendAreaTriggerMessage("Vous avez atteint le maximum de plantations");
						player->CLOSE_GOSSIP_MENU();

					}
				}
				else { // Si il n'as pas la money requise.
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez d'argent.");

				}


				break;

			case 10: // Fermer

				player->CLOSE_GOSSIP_MENU();

				break;
			}

		}

		return true;
	}


};
void AddSC_Vegetable_script()
{
	new GameObject_Vegetable;
	new NPC_Vegetable;
}