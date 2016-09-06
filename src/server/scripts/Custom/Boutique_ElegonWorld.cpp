#include "ScriptMgr.h"
#include "Group.h"
#include "Player.h"

void givetitle(Player * player, int id) // function to give a title
{
	if (player->HasTitle(sCharTitlesStore.LookupEntry(id)))
	{
		player->GetSession()->SendAreaTriggerMessage("Vous avez deja ce titre !");
		player->CLOSE_GOSSIP_MENU();
		return;
	}
	
	if (player->HasItemCount(456500, 10, true)) {
		player->SetTitle(sCharTitlesStore.LookupEntry(id));
		player->DestroyItemCount(456500, 10, true);
		player->GetSession()->SendAreaTriggerMessage("Merci de votre achat");
		player->CLOSE_GOSSIP_MENU();

	}
	else {
		player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas 10 Marques Elegon World.");
		player->CLOSE_GOSSIP_MENU();
	}
}

class Boutique_ElegonWorld_1 : public CreatureScript
{

public:
	Boutique_ElegonWorld_1() : CreatureScript("Boutique_ElegonWorld_1") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:30|t |cff3A01DF[Boutique ]:|r Titres custom", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{

			case 1:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Garde-paix", GOSSIP_SENDER_MAIN, 30);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Plaie-du-Fer", GOSSIP_SENDER_MAIN, 31);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Marcheur des esprits", GOSSIP_SENDER_MAIN, 32);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r le traquemort", GOSSIP_SENDER_MAIN, 33);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r le protecteur d'ame", GOSSIP_SENDER_MAIN, 34);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r le derailleur", GOSSIP_SENDER_MAIN, 35);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r lindomptable", GOSSIP_SENDER_MAIN, 36);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r l'esprit empoisonnee", GOSSIP_SENDER_MAIN, 37);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r l'abatteur", GOSSIP_SENDER_MAIN, 38);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r homme de guerre", GOSSIP_SENDER_MAIN, 39);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r au rire dement", GOSSIP_SENDER_MAIN, 40);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r assaillant confirmee", GOSSIP_SENDER_MAIN, 41);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Architecte", GOSSIP_SENDER_MAIN, 42);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Clairvoyant", GOSSIP_SENDER_MAIN, 43);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Precheuse", GOSSIP_SENDER_MAIN, 44);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Archonte", GOSSIP_SENDER_MAIN, 45);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Mastodonte", GOSSIP_SENDER_MAIN, 46);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r l'Arachnoide ", GOSSIP_SENDER_MAIN, 47);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Grand joueur", GOSSIP_SENDER_MAIN, 48);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Meilleur joueur du monde", GOSSIP_SENDER_MAIN, 49);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Le saint graal", GOSSIP_SENDER_MAIN, 50);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Necrophile", GOSSIP_SENDER_MAIN, 51);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Le maître du monopoly", GOSSIP_SENDER_MAIN, 52);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_quests_completed_04:20|t |cff3A01DF[10 Marques Elegon World]:|r Farmeur chinois", GOSSIP_SENDER_MAIN, 53);
				player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
				break;
			case 3:
				break;
			case 4:
				break;
			case 30:
				givetitle(player, 234);
				break;
			case 31:
				givetitle(player, 233);
				break;
			case 32:
				givetitle(player, 232);
				break;
			case 33:
				givetitle(player, 231);
				break;
			case 34:
				givetitle(player, 230);
				break;
			case 35:
				givetitle(player, 229);
				break;
			case 36:
				givetitle(player, 228);
				break;
			case 37:
				givetitle(player, 227);
				break;
			case 38:
				givetitle(player, 226);
				break;
			case 39:
				givetitle(player, 225);
				break;
			case 40:
				givetitle(player, 224);
				break;
			case 41:
				givetitle(player, 223);
				break;
			case 42:
				givetitle(player, 222);
				break;
			case 43:
				givetitle(player, 221);
				break;
			case 44:
				givetitle(player, 220);
				break;
			case 45:
				givetitle(player, 219);
				break;
			case 46:
				givetitle(player, 218);
				break;
			case 47:
				givetitle(player, 195);
				break;
			case 48:
				givetitle(player, 194);
				break;
			case 49:
				givetitle(player, 193);
				break;
			case 50:
				givetitle(player, 192);
				break;
			case 51:
				givetitle(player, 191);
				break;
			case 52:
				givetitle(player, 189);
				break;
			case 53:
				givetitle(player, 188);
				break;
			case 54:
				givetitle(player, 187);
				break;
			case 500:
				if (player->HasItemCount(456500, 10))
				{
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat");
					player->AddItem(250000, 1);
					player->DestroyItemCount(456500, 10, true);
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas 10 Marques Elegon World.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 501:
				if (player->HasItemCount(456500, 10))
				{
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat");
					player->AddItem(250001, 1);
					player->DestroyItemCount(456500, 10, true);
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas 10 Marques Elegon World.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 502:
				if (player->HasItemCount(456500, 10))
				{
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat");
					player->AddItem(250002, 1);
					player->DestroyItemCount(456500, 10, true);
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas 10 Marques Elegon World.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 20:
				player->CLOSE_GOSSIP_MENU();
				break;
			}
		}
		return true;
	}


};
class Boutique_ElegonWorld : public CreatureScript
{

public:
	Boutique_ElegonWorld() : CreatureScript("Boutique_ElegonWorld") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		player->ADD_GOSSIP_ITEM(2, "|TInterface/ICONS/Achievement_quests_completed_04:30|t |cff3A01DF[Boutique ]:|r Tresorie", GOSSIP_SENDER_MAIN, 1);
		player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		return true;
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_coin_19:20|t |cff3A01DF[10 Marques Elegon World]:|r 25000 d'or", GOSSIP_SENDER_MAIN, 380);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_coin_18:20|t |cff3A01DF[20 Marques Elegon World]:|r 55000 d'or", GOSSIP_SENDER_MAIN, 381);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_coin_17:20|t |cff3A01DF[30 Marques Elegon World]:|r 150000 d'or", GOSSIP_SENDER_MAIN, 382);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_key_01:20|t |cff3A01DF[50 Marques Elegon World]:|r Acces Palier 3", GOSSIP_SENDER_MAIN, 384);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_key_02:20|t |cff3A01DF[75 Marques Elegon World]:|r Acces Palier 4", GOSSIP_SENDER_MAIN, 385);
				player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
				break;
			case 380:
				if (player->HasItemCount(456500, 10, true))
				{
					player->ModifyMoney(player->GetMoney() + 250000000, true);
					player->DestroyItemCount(456500, 10, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 381:
				if (player->HasItemCount(456500, 20, true))
				{
					player->ModifyMoney(player->GetMoney() + 550000000, true);
					player->DestroyItemCount(456500, 20, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marque.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 382:
				if (player->HasItemCount(456500, 30, true))
				{
					player->ModifyMoney(player->GetMoney() + 1500000000, true);
					player->DestroyItemCount(456500, 30, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 383:
				if (player->HasItemCount(456500, 100, true))
				{
					player->ModifyMoney(player->GetMoney() + 2000000000, true);
					player->DestroyItemCount(456500, 100, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 384:
				if (player->HasItemCount(456500, 50, true))
				{
					player->AddItem(100050, 1);
					player->DestroyItemCount(456500, 50, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 385:
				if (player->HasItemCount(456500, 75, true) && (player->HasItemCount(100050, 1, true))
					)
				{
					player->AddItem(100051, 1);
					player->DestroyItemCount(456500, 200, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();

				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques et vous n'avez pas l'acces palier 3.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			}
		}
		return true;
	}


};
class Boutique_ElegonWorld_2 : public CreatureScript
{

public:
	Boutique_ElegonWorld_2() : CreatureScript("Boutique_ElegonWorld_2") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_chest_leather_cataclysm_b_01:30|t 5x Token [Palier II] - |cff2EFE2EEquipements|r : 10 Points Elegon World", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_staff_54:30|t 10x Token [Palier II] - |cff2EFE2EArmes|r : 10 Points Elegon World", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_boots_05:30|t 3x Token [Palier II] - |cff2EFE2EHors-set|r : 10 Points Elegon World", GOSSIP_SENDER_MAIN, 3);
			player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		    return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(456500, 10))
				{
					player->AddItem(250000, 5);
					player->DestroyItemCount(456500, 10, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
			break;
			case 2:
				if (player->HasItemCount(456500, 10))
				{
					player->AddItem(250001, 10);
					player->DestroyItemCount(456500, 10, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 3:
				if (player->HasItemCount(456500, 10))
				{
					player->AddItem(250002, 3);
					player->DestroyItemCount(456500, 10, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			}
		}
		return true;
	}


};
class Boutique_ElegonWorld_Palier2 : public CreatureScript
{
public:
	Boutique_ElegonWorld_Palier2() : CreatureScript("Boutique_ElegonWorld_Palier2") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
		if (player->GetTotalPlayedTime() >= 36000 || player->isVIP()) {

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_chest_leather_cataclysm_b_01:30|t 5x Token [Palier III] - |cff0174DFEquipements|r : 30 Points Elegon World", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_staff_54:30|t 10x Token [Palier III] - |cff0174DFArmes|r : 30 Points Elegon World", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_boots_05:30|t 3x Token [Palier III] - |cff0174DFHors-set|r : 30 Points Elegon World", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_banner_02:30|t 75000 points d'honneur : 30 Points Elegon World", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_misc_token_honorhold:30|t 1000 points d'arene: 30 Points Elegon World", GOSSIP_SENDER_MAIN, 5);	
			player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		} 
		else {
			player->GetSession()->SendAreaTriggerMessage("Pour acceder a ce vendeur vous devez avoir au moins 10 heures de jeu ou devenir premium.");
			player->CLOSE_GOSSIP_MENU();

		}
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(456500, 30))
				{
					player->AddItem(250003, 5);
					player->DestroyItemCount(456500, 30, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 2:
				if (player->HasItemCount(456500, 30))
				{
					player->AddItem(250005, 10);
					player->DestroyItemCount(456500, 30, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 3:
				if (player->HasItemCount(456500, 30))
				{
					player->AddItem(250006, 3);
					player->DestroyItemCount(456500, 30, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 4:
				if (player->HasItemCount(456500, 30))
				{
					player->SetHonorPoints(player->GetHonorPoints() + 75000);
					player->DestroyItemCount(456500, 30, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 5:
				if (player->HasItemCount(456500, 30))
				{
					player->SetArenaPoints(player->GetArenaPoints() + 1000);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			}
		}
		return true;
	}


};
class Boutique_ElegonWorld_Palier3 : public CreatureScript
{
public:
	Boutique_ElegonWorld_Palier3() : CreatureScript("Boutique_ElegonWorld_Palier3") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
		if (player->GetTotalPlayedTime() >= 108000 || player->isVIP()) {

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_chest_leather_cataclysm_b_01:30|t 5x Token [Palier IV] - |cff0174DFEquipements|r : 100 Points Elegon World", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_staff_54:30|t 10x Token [Palier IV] - |cff0174DFArmes|r : 100 Points Elegon World", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_boots_05:30|t 3x Token [Palier IV] - |cff0174DFHors-set|r : 100 Points Elegon World", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/inv_boots_05:30|t Accessoires palier 4", GOSSIP_SENDER_MAIN, 4);
			player->SEND_GOSSIP_MENU(25002, creature->GetGUID());
		}
		else {

			player->GetSession()->SendAreaTriggerMessage("Pour acceder a ce vendeur vous devez avoir au moins 30 heures de jeu ou devenir premium.");
			player->CLOSE_GOSSIP_MENU();

		}
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				if (player->HasItemCount(456500, 50))
				{
					player->AddItem(250007, 5);
					player->DestroyItemCount(456500, 50, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 2:
				if (player->HasItemCount(456500, 50))
				{
					player->AddItem(250008, 10);
					player->DestroyItemCount(456500, 50, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 3:
				if (player->HasItemCount(456500, 50))
				{
					player->AddItem(250009, 3);
					player->DestroyItemCount(456500, 50, true);
					player->GetSession()->SendAreaTriggerMessage("Merci de votre achat.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques.");
					player->CLOSE_GOSSIP_MENU();
				}
				break;
			case 4:
				player->GetSession()->SendListInventory(55512);
				break;
			}
		}
		return true;
	}


};
class item_boutique_repair : public ItemScript
{
public:
	item_boutique_repair() : ItemScript("item_boutique_repair") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->GetSession()->SendAreaTriggerMessage("Ok !");
		player->DurabilityRepairAll(0, 0, 0);
		return false;
	}
};
class item_boutique_bank : public ItemScript
{
public:
	item_boutique_bank() : ItemScript("item_boutique_bank") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->GetSession()->SendShowBank(player->GetGUID());
		return false;
	}
};
class item_boutique_race : public ItemScript
{
public:
	item_boutique_race() : ItemScript("item_boutique_race") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->GetSession()->SendAreaTriggerMessage("Pour effectuer le changement, merci de vous deconnecter.");
		player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		return false;
	}
};
class item_boutique_changeFaction : public ItemScript
{
public:
	item_boutique_changeFaction() : ItemScript("item_boutique_changeFaction") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->GetSession()->SendAreaTriggerMessage("Pour effectuer le changement, merci de vous deconnecter.");
		player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		return false;
	}
};
class item_boutique_customize : public ItemScript
{
public:
	item_boutique_customize() : ItemScript("item_boutique_customize") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->GetSession()->SendAreaTriggerMessage("Pour effectuer le changement, merci de vous deconnecter.");
		player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		return false;
	}
};
class item_boutique_size : public ItemScript
{
public:
	item_boutique_size() : ItemScript("item_boutique_size") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->SetObjectScale(2.f);
		return false;
	}
};
void AddSC_boutique_script()
{
	new Boutique_ElegonWorld();
	new Boutique_ElegonWorld_1();
	new Boutique_ElegonWorld_2();
	new item_boutique_repair();
	new item_boutique_bank();
	new item_boutique_changeFaction();
	new item_boutique_customize();
	new item_boutique_race();
	new item_boutique_size();
	new Boutique_ElegonWorld_Palier2();
	new Boutique_ElegonWorld_Palier3();

}