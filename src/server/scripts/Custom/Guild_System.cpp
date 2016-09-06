#include "ScriptMgr.h"
#include <string>



#define COOR_X 20
#define COOR_Y 20
#define COOR_Z 20



// Fin des définitions

// Level max de guilde
#define LEVEL_MAX 20

int REWARD_TAKE_QG = 250;
int COST_HOUSE_GUILD = 20000;

class NPC_House_Master_guilde : public CreatureScript  // House main
{
public:
	NPC_House_Master_guilde() : CreatureScript("NPC_House_Master_guilde") {}
	
	bool OnGossipHello(Player *player, Creature *creature)
	{

		
		/* Fonctionnalité du gamemaster */
		if (player->GetGuild()) { // Si le joueur n'as pas de guilde


			QueryResult result_data_guild = CharacterDatabase.PQuery("SELECT guildid, name, exp, lvl, has_house_guild, type FROM guild WHERE guildid = %u", player->GetGuildId());
			if (!result_data_guild)
			{
				return false;
			}

			Field *fields = result_data_guild->Fetch();
			uint32 guildid = fields[0].GetUInt32();
			std::string name = fields[1].GetString();
			uint32 exp = fields[2].GetUInt32();
			uint32 lvl = fields[3].GetUInt32();
			uint32 has_house_guild = fields[4].GetUInt32();
			uint32 type = fields[5].GetUInt32();


			
			

			std::stringstream data_guild_info;
			data_guild_info << "----------------------- " << std::endl;
			data_guild_info << "|cffDF3A01GUILDE|r : " << name << std::endl;
			data_guild_info << "Niveau : " << lvl << std::endl;
			data_guild_info << "Experiences : " << exp << std::endl;
			data_guild_info << "----------------------- " << std::endl;


			std::stringstream data_guild_lvl;
			
			int _niveau_guild = lvl + 1;
			int _niveau_requis = lvl * 1000;

			data_guild_lvl << "----------------------- " << std::endl;
			data_guild_lvl << "Niveau suivant : " << _niveau_guild << std::endl;
			data_guild_lvl << "Experience requise : " << _niveau_requis << std::endl;
			data_guild_lvl << "----------------------- " << std::endl;

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, data_guild_info.str() , GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, data_guild_lvl.str(), GOSSIP_SENDER_MAIN, 2);
			if (type == 0) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Choisir le type de guilde (JcE/JcJ)", GOSSIP_SENDER_MAIN, 3);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Monter de niveau la guilde", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Liste des QG de la guilde", GOSSIP_SENDER_MAIN, 6);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Comment gagner de l'experience de guilde ?", GOSSIP_SENDER_MAIN, 12);

			if (has_house_guild == 0) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Acheter une maison de guilde", GOSSIP_SENDER_MAIN, 7);
			}
			if (has_house_guild == 1) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Teleporation a la maison de guilde", GOSSIP_SENDER_MAIN, 8);
			}

			if (type == 1) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Recompenses de niveau de guilde (JcJ)", GOSSIP_SENDER_MAIN, 9);
			}
			if (type == 2) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Recompenses de niveau de guilde (JcE)", GOSSIP_SENDER_MAIN, 10);

			}
			if (type == 0) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Votre guilde n'as pas encore choisis son type (JcJ/JcE)", GOSSIP_SENDER_MAIN, 11);
			}
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			return true;
			

		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de guilde");
			player->CLOSE_GOSSIP_MENU();
		}
		

	}

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		QueryResult result_data_guild = CharacterDatabase.PQuery("SELECT guildid, name, exp, lvl, has_house_guild, type FROM guild WHERE guildid = %u", player->GetGuildId());
		if (!result_data_guild)
		{
			return false;
		}

		Field *fields = result_data_guild->Fetch();
		uint32 guildid = fields[0].GetUInt32();
		std::string name = fields[1].GetString();
		uint32 exp = fields[2].GetUInt32();
		uint32 lvl = fields[3].GetUInt32();
		uint32 has_house_guild = fields[4].GetUInt32();
		uint32 type = fields[5].GetUInt32();

		int _niveau_guild = lvl + 1;
		int _niveau_requis = lvl * 1000;

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 3:
				player->ADD_GOSSIP_ITEM(0, "Type JcE (Vous aurez des bonus de type PvE)", GOSSIP_SENDER_MAIN, 1002);
				player->ADD_GOSSIP_ITEM(0, "Type JcJ (Vous aurez des bonus de type joueur contre joueur)", GOSSIP_SENDER_MAIN, 1001);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());

				break;
			case 4:
				if (exp >= _niveau_requis) {

					CharacterDatabase.PQuery("UPDATE guild SET lvl = lvl + 1 WHERE guildid = %u", player->GetGuildId()); //
					CharacterDatabase.PQuery("UPDATE guild SET exp = exp - %u WHERE guildid = %u", _niveau_requis, player->GetGuildId()); // 
					player->GetSession()->SendAreaTriggerMessage("Felecitation votre guilde est maintenant niveau : %u", _niveau_guild);
					player->CLOSE_GOSSIP_MENU();
					
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez d'experience de guilde!");
					player->CLOSE_GOSSIP_MENU();

				}
				break;
			case 6: {

						QueryResult result_data_qg = WorldDatabase.PQuery("SELECT name_qg FROM qg_system WHERE guild_id = %u", player->GetGuildId());

						if (!result_data_qg) {
							player->PlayerTalkClass->ClearMenus();
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Aucun QG", GOSSIP_SENDER_MAIN, 1000);
							player->SEND_GOSSIP_MENU(1, creature->GetGUID());
						}
						else {
							do {
								Field *fields = result_data_qg->Fetch();
								std::string name = fields[0].GetString();
								uint32 rank = 1;

								std::stringstream buffer;
								buffer << rank << ". [" << name << "] " << std::endl;
								player->PlayerTalkClass->ClearMenus();
								player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, rank);
								rank++;
							} while (result_data_qg->NextRow());
							player->SEND_GOSSIP_MENU(1, creature->GetGUID());

						}
					}

				break;

			case 7:
				if (player->GetMoney() > COST_HOUSE_GUILD)  { // On vérifie si il a le prix

					WorldDatabase.PQuery("INSERT INTO house_guild_system (guildid, work, contribution, cote) VALUES (%u, 0, 0, 0)", player->GetGuildId()); // On applique la nouvelle ID
					CharacterDatabase.PQuery("UPDATE guild SET has_house_guild = has_house_guild + 1 WHERE guildid = %u", player->GetGuildId()); // 
					player->GetSession()->SendAreaTriggerMessage("Merci ! Pour vous rendre a votre maison de guilde. Teleportez vous !");
					player->CLOSE_GOSSIP_MENU();
					player->ModifyMoney(player->GetMoney() - COST_HOUSE_GUILD, true);

				}
				else {

					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez d'argent");
					player->CLOSE_GOSSIP_MENU();


				}


			break;

			case 8: {

						QueryResult result_data_guild = WorldDatabase.PQuery("SELECT id FROM house_guild_system WHERE guildid = %u", player->GetGuildId());

						Field *fields = result_data_guild->Fetch();
						uint32 id = fields[0].GetUInt32();

						if (id == 1) {

							player->TeleportTo(37, 0, 0, 0, 0); // Téléportation Maison de guilde _1

						}
						if (id == 2) {

							player->TeleportTo(37, 0, 0, 0, 0); // Téléportation Maison de guilde _1

						}
						if (id == 3) {


							player->GetSession()->SendAreaTriggerMessage("Zone en construction !");
							player->CLOSE_GOSSIP_MENU();
						}

						break;
			}
			case 1000:
				player->CLOSE_GOSSIP_MENU();
				break;
			case 1001:
				CharacterDatabase.PQuery("UPDATE guild SET type = type + 1 WHERE guildid = %u", player->GetGuildId()); // Type 1
				player->CLOSE_GOSSIP_MENU();
				break;
			case 1002: 
				CharacterDatabase.PQuery("UPDATE guild SET type = type + 2 WHERE guildid = %u", player->GetGuildId()); // Type 1
				player->CLOSE_GOSSIP_MENU();
				break;

			}

		}
		return true;
	}



};

class NPC_House_guild_main : public CreatureScript // Maître des maison de guilde.
{

	public:
		NPC_House_guild_main() : CreatureScript("NPC_House_guild_main") {}


		bool OnGossipHello(Player *player, Creature *creature)
		{
			QueryResult result_data_guild = WorldDatabase.PQuery("SELECT id, guildid, work, contribution, cote FROM house_guild_system WHERE guildid = %u", player->GetGuildId());

			Field *fields = result_data_guild->Fetch();
			uint32 id = fields[0].GetUInt32();
			uint32 guildid = fields[1].GetUInt32();
			uint32 work = fields[2].GetUInt32();
			uint32 contribution = fields[3].GetUInt32();
			uint32 cote = fields[4].GetUInt32();

			std::stringstream data_guild_house;
			data_guild_house << "---------|cff74DF00INFORMATIONS|r---- " << std::endl;
			data_guild_house << "Contribution : " << contribution << std::endl;
			data_guild_house << "Main d'oeuvre : " << work << std::endl;
			data_guild_house << "Cote de la maison : " << cote << std::endl;
			data_guild_house << "----------------------- " << std::endl;

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, data_guild_house.str(), GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Acheter de la main d'oeuvre", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Faire une contribution", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Classement", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Faire une demande de (GvG)", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Comment est-ce que ca marche ?", GOSSIP_SENDER_MAIN, 5);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		}

};


class GOB_Capture_GvG : public GameObjectScript
{
public :

	GOB_Capture_GvG() : GameObjectScript("GOB_Capture_GvG") {}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		if (player->GetGuild()) { // Si il n'as pas de guilde
			
			player->ADD_GOSSIP_ITEM(2, "Capture", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			return true;

		}

		else {

			player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de guilde");
			player->CLOSE_GOSSIP_MENU();
		}


	}

	bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				QueryResult result_data_qg = WorldDatabase.PQuery("SELECT guild_id, name_qg FROM qg_system WHERE id_zone = %u", player->GetZoneId());

				Field *fields = result_data_qg->Fetch();
				uint32 guild_id = fields[0].GetUInt32();
				std::string name_qg = fields[1].GetString();

				std::string name_guild = player->GetGuildName();

				if (guild_id == player->GetGuildId()) {

					player->GetSession()->SendAreaTriggerMessage("Ce QG vous appartient deja !");
					player->CLOSE_GOSSIP_MENU();

				}
				else {

					if (IsEventActive(100)) { 
						WorldDatabase.PQuery("UPDATE qg_system SET guild_id = %u WHERE id_zone = %u", player->GetGuildId(), player->GetZoneId()); // On applique la nouvelle ID
						CharacterDatabase.PQuery("UPDATE guild SET exp = exp + %u WHERE guildid = %u", REWARD_TAKE_QG, player->GetGuildId()); // On ajoute 250 points.
					}
					else {
						player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas encore prendre ce QG!");
						player->CLOSE_GOSSIP_MENU();
					}
				
				}

			break;
			}
		}
		return true;
	}

};


// 2 paramètres possible (Si il meurt mais qu'il n'a pas le qg, ou l'inverse

class OnPlayerDeadZone : public PlayerScript
{

public :

	OnPlayerDeadZone() : PlayerScript("OnPlayerDeadZone") {}

	void OnPVPKill(Player* killer, Player* killed)
	{

		if (killer == killed) {
			return; // Si la personne ce tue
		}


		if (killed->GetGuild()) {

			QueryResult result_data_qg = WorldDatabase.PQuery("SELECT guild_id, name_qg FROM qg_system WHERE id_zone = %u", killed->GetZoneId());
			if (!result_data_qg)
				return;

			Field *fields = result_data_qg->Fetch();
			uint32 guild_id = fields[0].GetUInt32();



			if (IsEventActive(100)) {

				if (killed->GetZoneId() == 11)  {

					killed->TeleportTo(0, 0, 0, 0, 0);
					killed->ResurrectPlayer(100, true);

					if (killed->GetGuildId() == guild_id) {

						killed->TeleportTo(0, 0, 0, 0, 0);
						killed->ResurrectPlayer(100, true);
					}

				}

				if (killed->GetZoneId() == 144)  {

					killed->TeleportTo(0, 0, 0, 0, 0);
					killed->ResurrectPlayer(100, true);

					if (killed->GetGuildId() == guild_id) {

						killed->TeleportTo(0, 0, 0, 0, 0);
						killed->ResurrectPlayer(100, true);

					}

				}

				if (killed->GetZoneId() == 42)  {

					killed->TeleportTo(0, 0, 0, 0, 0);
					killed->ResurrectPlayer(100, true);

					if (killed->GetGuildId() == guild_id) {

						killed->TeleportTo(0, 0, 0, 0, 0);
						killed->ResurrectPlayer(100, true);

					}

				}

				if (killed->GetZoneId() == 2418)  {

					killed->TeleportTo(0, 0, 0, 0, 0); // Si il n'a pas le QG
					killed->ResurrectPlayer(100, true);

					if (killed->GetGuildId() == guild_id) {

						killed->TeleportTo(0, 0, 0, 0, 0); // Si il dispose du QG
						killed->ResurrectPlayer(100, true);

					}

				}


			}
			else { // Dés que l'Event n'est plus actif

				killed->TeleportTo(0, 0, 0, 0, 0);
				killed->ResurrectPlayer(100, true);

			}
		}
	}

};
void AddSC_GuildScript_script()
{

	new NPC_House_Master_guilde();
	new NPC_House_guild_main();
	new GOB_Capture_GvG();
	new OnPlayerDeadZone();
}