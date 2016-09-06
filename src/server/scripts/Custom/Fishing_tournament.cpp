
class Fishing_NPC : public CreatureScript
{
public:

	Fishing_NPC() : CreatureScript("Fishing_NPC") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		QueryResult result_Fishing = WorldDatabase.PQuery("SELECT guid, nbr FROM fishing WHERE guid = %u", player->GetGUID());
		
		if (!result_Fishing) { // Si il n'existe pas, on l'inscris.

			player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Achievement_profession_fishing_outlandangler:20|t |cffFF0000J'aimerais m'inscrire dans le tournois de peche !|r", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(2, "Au revoir", GOSSIP_SENDER_MAIN, 10);
			player->SEND_GOSSIP_MENU(36870, creature->GetGUID());
			return true;

		} // Sinon on l'affiche les points en cours.
				Field *fields = result_Fishing->Fetch();
		uint32 guid = fields[0].GetUInt32();
		uint32 nbr = fields[1].GetUInt32();

		std::stringstream info_house;
		info_house << "|TInterface/ICONS/Achievement_profession_fishing_findfish:20|t |cff0174DF[Tournois] :|r Vos points : " << nbr << std::endl;
		player->ADD_GOSSIP_ITEM(2, info_house.str(), GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Achievement_profession_fishing_journeymanfisher:20|t |cff0174DF[Tournois] :|r Classement", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(2, "Au revoir", GOSSIP_SENDER_MAIN, 10);
		player->SEND_GOSSIP_MENU(36871, creature->GetGUID());
	
	}

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1: 

						WorldDatabase.PQuery("UPDATE fishing_event SET date_start = CURRENT_DATE, date_end = CURRENT_DATE + INTERVAL 1 DAY");
						WorldDatabase.PQuery("INSERT INTO fishing (guid, name_player) VALUES (%u, '%s')", player->GetGUID(), player->GetName()); // On l'inscris dans le tournois.
							player->CLOSE_GOSSIP_MENU();
			break;

			case 3:
				QueryResult result = WorldDatabase.PQuery("SELECT guid, nbr, name_player FROM fishing ORDER BY nbr DESC");
				player->CLOSE_GOSSIP_MENU();
				
				do {

					Field *fields = result->Fetch();
					uint32 guid = fields[0].GetUInt32();
					uint32 nbr = fields[1].GetUInt32();
					std::string  name = fields[2].GetString();
					char msg[250];
					snprintf(msg, 250, "|cffDBA901Nom du joueur : |r |cffB40431%s|r . |cff0174DFpoints :|r |cffDF01A5%u|r", name, nbr);
					player->MonsterWhisper(msg, player);

				} while (result->NextRow());

				player->PlayerTalkClass->ClearMenus();
				player->SEND_GOSSIP_MENU(0, creature->GetGUID());
				break;
			}
		}
		return true;
	}

};

class OnPlayerSaveFishing : public PlayerScript
{

public:

	OnPlayerSaveFishing() : PlayerScript("OnPlayerSaveFishing") {}

	void OnSave(Player* player)
	{
		QueryResult result_Fishing_event_end = WorldDatabase.PQuery("SELECT DAY(date_end) AS jour FROM fishing_event");

		if (!result_Fishing_event_end)
			return;

		Field *fields1 = result_Fishing_event_end->Fetch();
		uint64 date_end = fields1[0].GetUInt64();

		QueryResult result_Fishing_event_end_day = CharacterDatabase.PQuery("SELECT DAY(date_day) AS jour FROM characters WHERE guid = %u", player->GetGUID());

		if (!result_Fishing_event_end_day)
			return;

		Field *fields = result_Fishing_event_end_day->Fetch();
		uint64 date_end_day = fields[0].GetUInt64();

		if (date_end == date_end_day) // Si c'est le bon jour alors =>
		{
			QueryResult result_Fishing_event_end = WorldDatabase.PQuery("SELECT guid, nbr FROM fishing  ORDER BY nbr DESC LIMIT 1", player->GetGUID());
			
			if (!result_Fishing_event_end_day)
				return;

			Field *fields1 = result_Fishing_event_end->Fetch();
			uint64 guid = fields1[0].GetUInt64();
			uint64 nbr = fields1[1].GetUInt64();

			if (player->GetGUID() == guid) { // RewardWinner
				player->ModifyMoney(+nbr * 100000);
				WorldDatabase.PQuery("DELETE FROM fishing");
				WorldDatabase.PQuery("UPDATE fishing_event SET date_start = CURRENT_DATE, date_end = CURRENT_DATE + INTERVAL 1 DAY");

			}

			sWorld->SendWorldText(55450);


		}

	}

};
void AddSC_Fishing_tournament()
{	
	new Fishing_NPC();
}