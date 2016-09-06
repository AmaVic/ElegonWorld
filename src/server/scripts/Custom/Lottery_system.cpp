#define MAX_CAGNOTTE 150000000000
#define PRIX_TICKET 1


class Lottery_system : public CreatureScript
{
private:

	int nombre;

public:
	Lottery_system() : CreatureScript("Lottery_system") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_guildperk_mobilebanking:30|t Voir la cagnotte en cours...", GOSSIP_SENDER_MAIN, 50);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_guildperk_cashflow_rank2:20|t Acheter un ticket de loterie.", GOSSIP_SENDER_MAIN, 1);
			if (player->IsGameMaster()) {
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_guildperk_cashflow_rank2:20|t Generer un nouveau code (ATTENTION)", GOSSIP_SENDER_MAIN, 2);
			}
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_guildperk_bartering:20|t Valider un ticket", GOSSIP_SENDER_MAIN, 3, "", 0, true);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Fermer", GOSSIP_SENDER_MAIN, 4);
			player->SEND_GOSSIP_MENU(25002, creature->GetGUID());

		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature * object, uint32 sender, uint32 action, char const* code)
	{

		std::string str = std::string(code);
		int nombre_convert = std::stoi(str);
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (action)
			{

				break;

			case 3: {

						QueryResult result_code = CharacterDatabase.PQuery("SELECT generated FROM lottery_winner");
						Field *fields_code = result_code->Fetch();
						uint32 generated = fields_code[0].GetUInt32();

						QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM lottery_system");
						Field *fields = result->Fetch();
						uint32 nbr_cagnotte = fields[0].GetUInt32();

						if (nombre_convert != generated) {
							player->GetSession()->SendAreaTriggerMessage("Perdu ! Retentez votre chance.");
							player->CLOSE_GOSSIP_MENU();
						}
						else { // Si il gagne alors =>

							QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM lottery_system WHERE numero = %u", nombre_convert);
							
							if (!result) {
								player->GetSession()->SendAreaTriggerMessage("Tricheur !");
								player->CLOSE_GOSSIP_MENU();
								return true;
							}

							Field *fields = result->Fetch();
							uint32 guid = fields[0].GetUInt32();
							if (player->GetGUID() == guid) {
								player->AddItem(456500, nbr_cagnotte);
								CharacterDatabase.PQuery("DELETE FROM lottery_system"); // Delete de tous les tickets
								uint64 generated_code = urand(1000, 100000); //Generation d'un nouveau code
								CharacterDatabase.PQuery("UPDATE lottery_winner SET generated = %u", generated_code);
								player->CLOSE_GOSSIP_MENU();
							} 
							else {

								player->GetSession()->SendAreaTriggerMessage("Perdu ! Retentez votre chance.");
								player->CLOSE_GOSSIP_MENU();
	
							}


						}


			}
				
			break;
			case 4:
				player->CLOSE_GOSSIP_MENU();
				break;

			}
		}
		return true;
	}
		

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT generated FROM lottery_winner");
		Field *fields = result->Fetch();
		uint32 generated = fields[0].GetUInt32();
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{



			case 50: {
						QueryResult result_nbr = CharacterDatabase.PQuery("SELECT COUNT(*) FROM lottery_system");
						Field *fields = result_nbr->Fetch();
						uint32 nbr_cagnotte  = fields[0].GetUInt32();
						player->GetSession()->SendAreaTriggerMessage("La cagnotte en cours est de : %u points Elegon World a gagner !", nbr_cagnotte);
						player->CLOSE_GOSSIP_MENU();
			}

			break;
			case 1: {

						QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM lottery_system");
						Field *fields = result->Fetch();
						uint32 nbr_cagnotte = fields[0].GetUInt32();

						if (nbr_cagnotte >= MAX_CAGNOTTE) {

							player->GetSession()->SendAreaTriggerMessage("Il n'y a plus de ticket a vendre revenez plus tard !");
							player->CLOSE_GOSSIP_MENU();

						}
						else {
							if (player->HasItemCount(456500, PRIX_TICKET, true)) {

								uint64 generated_new = generated + (urand(0, 100));
								CharacterDatabase.PQuery("INSERT INTO lottery_system (guid, numero, winner) VALUES (%u, %u, 0)", player->GetGUID(), generated_new);
								char msg[250];
								snprintf(msg, 250, "Voici votre code : %u vous pouvez soit le valider maintenant ou attendre", generated_new);
								player->MonsterWhisper(msg, player);
								player->CLOSE_GOSSIP_MENU();
								player->DestroyItemCount(456500, PRIX_TICKET, true);
							}
							else {
								player->CLOSE_GOSSIP_MENU();
								player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de points elegon-world");

							}

						}

			}

			break;
			case 2: {				
				uint64 generated_code = urand(1000, 100000);
				CharacterDatabase.PQuery("UPDATE lottery_winner SET generated = %u", generated_code);
				CharacterDatabase.PQuery("DELETE FROM lottery_system"); // Delete de tous les tickets
				player->CLOSE_GOSSIP_MENU(); 
			}

				break;
			}
		}
		return true;
	}
};
void AddSC_Lottery_script()
{
	new Lottery_system;

}