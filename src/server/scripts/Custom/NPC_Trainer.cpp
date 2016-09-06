#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include <string>
class SymbolixDEV_NPC : public CreatureScript
{
public:
	SymbolixDEV_NPC() : CreatureScript("SymbolixDEV_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes guerrier", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Paladin", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Chasseur", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Voleur", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Pretre", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Chevalier de la mort", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Chaman", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Mage", GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Demoniste", GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_general:30|t Glyphes Druide", GOSSIP_SENDER_MAIN, 10);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{ 

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500111);

				break;

			case 2:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500068);

				break;

			case 3:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500046);

				break;

			case 4:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500095);

				break;

			case 5:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500113);

				break;


			case 6:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500026);

				break;

			case 7:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500097);

				break;

			case 8:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500057);

				break;

			case 9:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500110);

				break;

			case 10:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500027);
				break;
			}

		}
		return true;
	
	}
};
class GlypheImpregnation : public CreatureScript
{
public:
	GlypheImpregnation() : CreatureScript("GlypheImpregnation") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{	
		switch (player->getClass())
		{
		case CLASS_WARRIOR:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes guerrier", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());

			break;
		case CLASS_PALADIN:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Paladin", GOSSIP_SENDER_MAIN, 2);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());

			break;
		case CLASS_HUNTER:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Chasseur", GOSSIP_SENDER_MAIN, 3);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());

			break;
		case CLASS_ROGUE:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Voleur", GOSSIP_SENDER_MAIN, 4);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());

			break;
		case CLASS_PRIEST:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Pretre", GOSSIP_SENDER_MAIN, 5);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());


			break;
		case CLASS_DEATH_KNIGHT:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Chevalier de la mort", GOSSIP_SENDER_MAIN, 6);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());


			break;
		case CLASS_SHAMAN:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Chaman", GOSSIP_SENDER_MAIN, 7);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());


			break;
		case CLASS_MAGE:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Mage", GOSSIP_SENDER_MAIN, 8);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());


			break;
		case CLASS_WARLOCK:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Demoniste", GOSSIP_SENDER_MAIN, 9);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());


			break;
		case CLASS_DRUID:
			player->ADD_GOSSIP_ITEM(0, "Liste des Glyphes Druide", GOSSIP_SENDER_MAIN, 10);
			player->SEND_GOSSIP_MENU(9000, creature->GetGUID());

			break;		
		}

		player->SEND_GOSSIP_MENU(9000, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {
			case 1:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de bataille", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de rage sanguinaire", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de charge", GOSSIP_SENDER_MAIN, 13);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de coup railleur", GOSSIP_SENDER_MAIN, 14);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de tourbillon", GOSSIP_SENDER_MAIN, 15);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de tourbillon", GOSSIP_SENDER_MAIN, 15);

				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 2:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 3:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 4:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 5:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 6:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 7:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 8:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 9:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
			case 10:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de regeneration frenetique", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de mutiler", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "|cff2E64FE[Impregnation]|r : Glyphe de grondement", GOSSIP_SENDER_MAIN, 13);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			}

		}
		return true;

	}
};
class Divers_NPC : public CreatureScript
{
public:
	Divers_NPC() : CreatureScript("Divers_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_drink_05:30|t Fournitures", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Ability_mount_snowygryphon:30|t Montures volante", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Ability_mount_blackpanther:30|t Montures terrestres", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_potion_10:30|t Elexirs", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_potion_03:30|t Potions", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_potion_115:30|t Flacons", GOSSIP_SENDER_MAIN, 7);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				player->GetSession()->SendListInventory(creature->GetGUID(), 19718);

				break;

			case 2:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500039);

				break;

			case 3:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500033);

				break;

			case 4:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500069);

				break;

			case 5:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500029);

				break;
			case 6:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500135);

				break;
			case 7:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500038);
				break;
			}

		}
		return true;

	}
};
class set_phase_on_die : public PlayerScript
{

public:

	set_phase_on_die() : PlayerScript("set_phase_on_die") { }

	void OnCreatureKill(Player* killer, Creature* killed) 
	{ 
		if (killed->GetEntry() == 67981) // Si c'est la créature en question
		{
			killer->SetPhaseMask(1, true); // Phase 0;
		}
	}
	void OnPlayerKilledByCreature(Creature* killer, Player* killed)
	{
		if (killer->GetEntry() == 67981) // Si c'est la créature en question
		{
			killer->DespawnOrUnsummon();
			killed->SetPhaseMask(1, true); // Phase 0;
			killed->ResurrectPlayer(100, false);
			killed->GetSession()->SendAreaTriggerMessage("Vous n'avez pas reussis, retentez le combat.");
		}
	}


};
class set_phase : public CreatureScript
{
public:

	set_phase() : CreatureScript("set_phase") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->GetQuestStatus(900908) == QUEST_STATUS_INCOMPLETE)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Commencer le duel", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Non merci", GOSSIP_SENDER_MAIN, 1);
			player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			return true;
		}

	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				if (player->FindNearestCreature(67981, 100.f)) // Si la creature existe déjà alors un autre joueur ne peux pas lancer un duel
				{
					player->CLOSE_GOSSIP_MENU();
					player->GetSession()->SendAreaTriggerMessage("Je suis deja en duel contre quelqu'un !");
				}
				else
				{
					Creature * Leryssa = player->SummonCreature(67981, creature->GetPosition(), TEMPSUMMON_CORPSE_DESPAWN);
					Leryssa->SetPhaseMask(2, true); // Phase mask 2 personne ne peux voir le joueur et la creature.
					player->SetPhaseMask(2, true);

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

class Gard_NPC : public CreatureScript
{
public:
	Gard_NPC() : CreatureScript("Gard_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment se teleporter ?", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment gagner de l'equipement ?", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Que sont les failles ?", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment obtenir les acces aux donjons ?", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Marche Elegon World, c'est quoi ?", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Les quetes journalieres", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t J'ai rencontre un probleme en jeu que dois je faire ?", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Au revoir", GOSSIP_SENDER_MAIN, 90);
		player->SEND_GOSSIP_MENU(26501, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {
			case 9000:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment se teleporter ?", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment gagner de l'equipement ?", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Que sont les failles ?", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Comment obtenir les acces aux donjons ?", GOSSIP_SENDER_MAIN, 4);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Marche Elegon World, c'est quoi ?", GOSSIP_SENDER_MAIN, 5);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t Les quetes journalieres", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_doublejeopardy:20|t J'ai rencontre un probleme en jeu que dois je faire ?", GOSSIP_SENDER_MAIN, 7);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Au revoir", GOSSIP_SENDER_MAIN, 90);
				player->SEND_GOSSIP_MENU(26501, creature->GetGUID());
				return true;
				break;

			case 1:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18900, creature->GetGUID());
				break;
			case 2:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18901, creature->GetGUID());
				break;
			case 3:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18902, creature->GetGUID());
				break;
			case 4:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18903, creature->GetGUID());
				break;
			case 5:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18904, creature->GetGUID());
				break;
			case 6:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18905, creature->GetGUID());
				break;
			case 7:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(0, "Retour", GOSSIP_SENDER_MAIN, 9000);
				player->SEND_GOSSIP_MENU(18906, creature->GetGUID());
				break;

			case 90:
				player->PlayerTalkClass->ClearMenus();
				break;

			}

		}
		return true;

	}
};



class Gemme_vendor : public CreatureScript
{
public:
	Gemme_vendor() : CreatureScript("Gemme_vendor") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_red:30|t Gemmes : Rouges", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_green:30|t Gemmes : Vertes", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_blue:30|t Gemmes : Bleues", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_orange:30|t Gemmes : Oranges", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_purple:30|t Gemmes : Violettes", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_silver:30|t Gemmes : Meta", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Inv_bijou_yellow:30|t Gemmes : Jaunes", GOSSIP_SENDER_MAIN, 7);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500092);

				break;

			case 2:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500041);

				break;

			case 3:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500014);

				break;

			case 4:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500067);

				break;

			case 5:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500073);

				break;
			case 6:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500059);

				break;
			case 7:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500112);

				break;

			}

		}
		return true;

	}
};
void set_mount(Player * player, int spell_mount)
{
	player->RemoveAurasDueToSpell(SPELL_AURA_MOUNTED);
	player->CastSpell(player, spell_mount);
	player->CLOSE_GOSSIP_MENU();

}
class Monture_test : public CreatureScript
{
public:
	Monture_test() : CreatureScript("Monture_test") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Tigre siberien", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Loup de guerre noir", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Loup de guerre rouge", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Loup de guerre mauve", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Loup de guerre jaune", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Panthere onyx vert", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Panthere onyx rouge", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Panthere onyx bleue", GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Panthere onyx jaune", GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01A9DB[Monture terrestre]|r : Cheval de guerre", GOSSIP_SENDER_MAIN, 18);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Serpent de jade bleu", GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Serpent de jade vert", GOSSIP_SENDER_MAIN, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Serpent de jade rouge", GOSSIP_SENDER_MAIN, 12);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Serpent de jade noir", GOSSIP_SENDER_MAIN, 13);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Gryphon Elite", GOSSIP_SENDER_MAIN, 16);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Spell_nature_swiftness:20|t|cff01DF3A[Monture volante]|r : Wyrmn	 Elite", GOSSIP_SENDER_MAIN, 17);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				set_mount(player, 80017);
				break;
			case 2:
				set_mount(player, 80004);
				break;
			case 3:
				set_mount(player, 90001);
				break;
			case 4:
				set_mount(player, 90002);
				break;
			case 5:
				set_mount(player, 90003);
				break;
			case 6:
				set_mount(player, 80008);
				break;
			case 7:
				set_mount(player, 80008);
				break;
			case 8:
				set_mount(player, 80006);
				break;
			case 9:
				set_mount(player, 80007);
				break;
			case 10:
				set_mount(player, 80012);
				break;
			case 11:
				set_mount(player, 80011);
				break;
			case 12:
				set_mount(player, 80005);
				break;
			case 13:
				set_mount(player, 80014);
				break;
			case 14:
				set_mount(player, 80005);
				break;
			case 16:
				set_mount(player, 80016);
				break;
			case 17:
				set_mount(player, 80015);
				break;
			case 18:
				set_mount(player, 80009);
				break;

			}

		}
		return true;

	}
};


class Marque_Echanger : public CreatureScript
{
public:
	Marque_Echanger() : CreatureScript("Marque_Echanger") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t[Echanger] : Marques JcE", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t[Echanger] : Marques JcJ", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:30|t[Echanger] : Marques Elegon World", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t 1 Marque palier 4 = 100 Marques du marche", GOSSIP_SENDER_MAIN, 10);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t 1 Marque palier 4 = 75000 points d'honneur.", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t 1 Marque palier 3 = 1 Marques du marche", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t 1 Marque palier 3 = 15000 points d'honneur", GOSSIP_SENDER_MAIN, 13);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
			case 2:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t 1500 points d'arene = 100 Marques du marche", GOSSIP_SENDER_MAIN, 20);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t 200000 points d'honneur = 100 Marques du marche", GOSSIP_SENDER_MAIN, 21);
				if (player->HasItemCount(100050))  {
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t 75000 points d'honneur et 50 points d'arene  =  1 Marque palier 3", GOSSIP_SENDER_MAIN, 24);
				}
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t 20000 points d'honneur = 5 Marques du marche", GOSSIP_SENDER_MAIN, 23);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
			case 20:
				if (player->GetArenaPoints() <= 1500) {
					player->AddItem(456501, 100);
					player->SetArenaPoints(player->GetArenaPoints() - 1500);
				player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas assez de points d'arene", player);
				player->CLOSE_GOSSIP_MENU();

				break;
			case 21:
				if (player->GetHonorPoints() <= 200000) {
					player->AddItem(456501, 100);
					player->SetHonorPoints(player->GetHonorPoints() - 200000);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas assez de points d'honneur", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 22:
				if (player->GetArenaPoints() <= 1500 && player->GetHonorPoints() <= 100000) {
					player->AddItem(250007, 1);
					player->AddItem(250008, 1);
					player->AddItem(250009, 1);
					player->SetHonorPoints(player->GetHonorPoints() - 100000);
					player->SetArenaPoints(player->GetArenaPoints() - 1500);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas assez de points d'arene ou de points d'honneur", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 23: {
						 if (player->GetHonorPoints() <= 20000) {
							 player->AddItem(456501, 5);
							 player->SetHonorPoints(player->GetHonorPoints() - 20000);
							 player->CLOSE_GOSSIP_MENU();
						 }
						 creature->MonsterWhisper("Vous n'avez pas assez de points d'honneur", player);
						 player->CLOSE_GOSSIP_MENU();
			}

				break;
			case 24:
				if (player->GetArenaPoints() <= 50 && player->GetHonorPoints() <= 7500) {

					player->AddItem(250003, 1);
					player->AddItem(250005, 1);
					player->AddItem(250006, 1);
					player->SetHonorPoints(player->GetHonorPoints() - 7500);
					player->SetArenaPoints(player->GetArenaPoints() - 50);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas assez de points d'arene ou de points d'honneur", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 3:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:30|t 10 Marques Elegon World = 5 Marques du marche", GOSSIP_SENDER_MAIN, 30);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:30|t 100 Marques Elegon World = 75 Marques du marche.", GOSSIP_SENDER_MAIN, 31);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:30|t 1000 Marques Elegon World = 1750 Marques du marche.", GOSSIP_SENDER_MAIN, 32);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
			case 30:
				if (player->HasItemCount(456500, 10))
				{

					player->AddItem(456501, 5);
					player->DestroyItemCount(456500, 10, true);
					player->CLOSE_GOSSIP_MENU();

				}
				creature->MonsterWhisper("Vous n'avez pas assez de marques Elegon World", player);
				player->CLOSE_GOSSIP_MENU();

				break;
			case 31:
				if (player->HasItemCount(456500, 100))
				{

					player->AddItem(456501, 75);
					player->DestroyItemCount(456500, 100, true);
					player->CLOSE_GOSSIP_MENU();

				}
				creature->MonsterWhisper("Vous n'avez pas assez de marques Elegon World", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 32:
				if (player->HasItemCount(456500, 1000))
				{

					player->AddItem(456501, 1750);
					player->DestroyItemCount(456500, 1000, true);
					player->CLOSE_GOSSIP_MENU();

				}
				creature->MonsterWhisper("Vous n'avez pas assez de marques Elegon World", player);
				player->CLOSE_GOSSIP_MENU();
				break;

				break;
			case 4:
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_reputation_argentcrusader:30|t[Echanger] : Marques JcE", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_pvp_p_250k:30|t[Echanger] : Marques JcJ", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:30|t[Echanger] : Marques Elegon World", GOSSIP_SENDER_MAIN, 3);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 10:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Équipements|r", GOSSIP_SENDER_MAIN, 50);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Armes|r", GOSSIP_SENDER_MAIN, 51);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Hors-set|r", GOSSIP_SENDER_MAIN, 52);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 11:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Équipements|r", GOSSIP_SENDER_MAIN, 60);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Armes|r", GOSSIP_SENDER_MAIN, 61);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier IV] - |cffB40404Hors-set|r", GOSSIP_SENDER_MAIN, 62);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 12:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAÉquipements|r", GOSSIP_SENDER_MAIN, 70);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAArmes|r", GOSSIP_SENDER_MAIN, 71);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAHors-set|r", GOSSIP_SENDER_MAIN, 72);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 13:
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAÉquipements|r", GOSSIP_SENDER_MAIN, 80);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAArmes|r", GOSSIP_SENDER_MAIN, 81);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "1 Token [Palier III] - |cff2ECCFAHors-set|r", GOSSIP_SENDER_MAIN, 82);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 4);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
				/* Marque du Marché -------------------------------------------------------*/
			case 50: if (player->HasItemCount(250007)) {			
					player->AddItem(456501, 100);
					player->DestroyItemCount(250007, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("1 Token [Palier IV] - |cffB40404Équipements|r", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 51: if (player->HasItemCount(250008)) {
					player->AddItem(456501, 100);
					player->DestroyItemCount(250008, 1, true);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas de Token [Palier IV] - |cffB40404Armes|r", player);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 52: if (player->HasItemCount(250009)) {
					player->DestroyItemCount(250009, 1, true);
					player->AddItem(456501, 100);
					player->CLOSE_GOSSIP_MENU();
				}
				creature->MonsterWhisper("Vous n'avez pas de Token [Palier IV] - |cffB40404Hors-set|r", player);
				player->CLOSE_GOSSIP_MENU();
				break;
				/* Marque du Marché -------------------------------------------------------*/

				/* HONNEUR -------------------------------------------------------*/
			case 60: if (player->HasItemCount(250007)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 75000);
						 player->DestroyItemCount(250007, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
			 creature->MonsterWhisper("1 Token [Palier IV] - |cffB40404Équipements|r", player);
			 player->CLOSE_GOSSIP_MENU();
					 break;


			case 61: if (player->HasItemCount(250008)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 75000);
						 player->DestroyItemCount(250008, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier IV] - |cffB40404Armes|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;
			case 62: if (player->HasItemCount(250009)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 75000);
						 player->AddItem(456501, 100);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier IV] - |cffB40404Hors-set|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;
					 /* HONNEUR -------------------------------------------------------*/


			case 70: if (player->HasItemCount(250003, 1)) {
						 player->AddItem(456501, 1);
						 player->DestroyItemCount(250003, 1, true);
						 player->CLOSE_GOSSIP_MENU();
					 }
					 creature->MonsterWhisper("1 Token [Palier III] - |cffB40404Équipements|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;
			case 71: if (player->HasItemCount(250005, 1)) {
						 player->AddItem(456501, 1);
						 player->DestroyItemCount(250005, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier III] - |cffB40404Arme|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;
			case 72: 
				if (player->HasItemCount(250006, 1)) {
						 player->AddItem(456501, 1);
						 player->DestroyItemCount(250006, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier III] - |cffB40404Hors-set|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;


			case 80: if (player->HasItemCount(250003)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 10000);
						 player->DestroyItemCount(250003, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("1 Token [Palier III] - |cffB40404Équipements|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;

			case 81: if (player->HasItemCount(250005)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 10000);
						 player->DestroyItemCount(250005, 1, true);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier III] - |cffB40404Armes|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;
			case 82: if (player->HasItemCount(250006)) {
						 player->SetHonorPoints(player->GetHonorPoints() + 10000);
						 player->AddItem(250009, 100);
						 player->CLOSE_GOSSIP_MENU();
			}
					 creature->MonsterWhisper("Vous n'avez pas de Token [Palier III] - |cffB40404Hors-set|r", player);
					 player->CLOSE_GOSSIP_MENU();
					 break;

			}

		}
		return true;

	}
};

class Professions_NPC : public CreatureScript
{
public:
	Professions_NPC() : CreatureScript("Professions_NPC") {}

	uint32 PlayerMaxLevel() const
	{
		return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
	}

	bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
	{
		return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
	}

	bool OnGossipHello(Player * pPlayer, Creature* _creature)
	{
		pPlayer->ADD_GOSSIP_ITEM(9, "[Professions] ->", GOSSIP_SENDER_MAIN, 196);
		pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
		return true;
	}

	bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
	{
		uint32 skillCount = 0;

		if (pPlayer->HasSkill(SKILL_MINING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_SKINNING))
			skillCount++;
		if (pPlayer->HasSkill(SKILL_HERBALISM))
			skillCount++;

		if (skillCount >= 2)
			return true;

		for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
		{
			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
			if (!SkillInfo)
				continue;

			if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
				continue;

			if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
				continue;

			const uint32 skillID = SkillInfo->id;
			if (pPlayer->HasSkill(skillID))
				skillCount++;

			if (skillCount >= 2)
				return true;
		}
		return false;
	}

	bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
	{
		if (!pPlayer->HasItemCount(456500, 30)){
			pPlayer->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques");
			return false;
		}
			
	
		ChatHandler handler(pPlayer->GetSession());
		char* skill_name;

		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
		skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];


		LearnSkillRecipesHelper(pPlayer, SkillInfo->id);

		pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 450, 450);
		pPlayer->DestroyItemCount(456500, 30, true);
		return true;
	}

	void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
	{
		uint32 classmask = player->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
		{
			SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
			if (!skillLine)
				continue;

			// wrong skill
			if (skillLine->skillId != skill_id)
				continue;

			// not high rank
			if (skillLine->forward_spellid)
				continue;

			// skip racial skills
			if (skillLine->racemask != 0)
				continue;

			// skip wrong class skills
			if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
				continue;

			SpellInfo const * spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
			if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
				continue;

			player->learnSpell(skillLine->spellId, false);
		}
	}

	bool IsSecondarySkill(SkillType skill) const
	{
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
	}

	void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
	{

			if (!LearnAllRecipesInProfession(pPlayer, skill)) {

			}
	}

	bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus();

		if (uiSender == GOSSIP_SENDER_MAIN)
		{

			switch (uiAction)
			{
			case 196:
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_alchemy:30|t Alchimie (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 1);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Ingot_05:30|t Forge (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 2);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_LeatherScrap_02:30|t Travail du cuir (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 3);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Fabric_Felcloth_Ebon:30|t Tailleur (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 4);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_wrench_01:30|t Ingenieur (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 5);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Scroll_08:30|t Calligraphie (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_Herb_07:30|t Herboriste (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 9);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_pelt_wolf_01:30|t Depecage (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 10);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Minage (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 11);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Peche (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 13);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Enchantement (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 14);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Joallier (Competence a 450 - 30 Marques Elegon World)", GOSSIP_SENDER_MAIN, 15);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/ICONS/Thrown_1H_Harpoon_D_01Blue:30|t Fermer", GOSSIP_SENDER_MAIN, 12);
				pPlayer->PlayerTalkClass->SendGossipMenu(1, _creature->GetGUID());
				break;
			case 1:
				if (pPlayer->HasSkill(SKILL_ALCHEMY))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_ALCHEMY);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 2:
				if (pPlayer->HasSkill(SKILL_BLACKSMITHING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_BLACKSMITHING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 3:
				if (pPlayer->HasSkill(SKILL_LEATHERWORKING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_LEATHERWORKING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 4:
				if (pPlayer->HasSkill(SKILL_TAILORING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_TAILORING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 5:
				if (pPlayer->HasSkill(SKILL_ENGINEERING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 6:
				if (pPlayer->HasSkill(SKILL_ENCHANTING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 7:
				if (pPlayer->HasSkill(SKILL_JEWELCRAFTING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 8:
				if (pPlayer->HasSkill(SKILL_INSCRIPTION))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}
				CompleteLearnProfession(pPlayer, _creature, SKILL_INSCRIPTION);

				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 9:
				if (pPlayer->HasSkill(SKILL_HERBALISM))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_HERBALISM);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 10:
				if (pPlayer->HasSkill(SKILL_SKINNING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_SKINNING);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 11:
				if (pPlayer->HasSkill(SKILL_MINING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_MINING);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;

			case 13:
				if (pPlayer->HasSkill(SKILL_ENCHANTING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 14:
				if (pPlayer->HasSkill(SKILL_FISHING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_FISHING);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			case 15:
				if (pPlayer->HasSkill(SKILL_JEWELCRAFTING))
				{
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				}

				CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;

			case 12:
				pPlayer->PlayerTalkClass->SendCloseGossip();
				break;
			}


		}
		return true;
	}
};

class ReliqueUlda : public GameObjectScript {


public:
	ReliqueUlda() : GameObjectScript("ReliqueUlda"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		player->ADD_GOSSIP_ITEM(2, "|TInterface\\icons\\trade_alchemy:20|t |cff01DF01Debloquer la relique|r", GOSSIP_SENDER_MAIN, 1);
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
				switch (urand(2,4))
				{
				case 2:
					player->AddAura(41107, player);
					go->Delete();
				break;
				case 3:
					player->AddAura(71953, player);
					go->Delete();

					break;
				case 4:
					player->AddAura(40733, player);
					go->Delete();

					break;
				}

			break;
			}

		}

		return true;
	}

};
class TabletteUlda : public GameObjectScript {


public:
	TabletteUlda() : GameObjectScript("TabletteUlda"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		player->ADD_GOSSIP_ITEM(0, "Debloquer la tablette", GOSSIP_SENDER_MAIN, 1);
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
				switch (urand(2, 4))
			case 2:
				go->CastSpell(player, 90000);
				go->Delete();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas reussis a la debloquer !");
				break;
			case 3:
				go->Delete();
				player->GetSession()->SendAreaTriggerMessage("La tablette s'est fracturée !");
				break;
			case 4:
				go->Delete();
				player->GetSession()->SendAreaTriggerMessage("La tablette s'est fracturée !");
				break;
			break;
			}

		}

		return true;
	}

};
class Bidule : public GameObjectScript {

private :

	int nombre;
	bool isNombre = false;

public:
	Bidule() : GameObjectScript("Bidule"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		if (player->GetItemCount(323202, 12, false)) {

			if (player->GetQuestStatus(200219) == QUEST_STATUS_INCOMPLETE) {

				if (isNombre == false) {
					nombre = urand(1, 50);
					isNombre = true;
				}

				player->ADD_GOSSIP_ITEM_EXTENDED(0, "Tenter de construire le bidule X41B67", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "", 0, true);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
			else {
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
			}
		}
		else {
			player->CLOSE_GOSSIP_MENU();
			player->GetSession()->SendAreaTriggerMessage("Erreur : vous n'avez pas les items requis !");

		}
		return true;
	}
	bool OnGossipSelectCode(Player* player, GameObject * object, uint32 sender, uint32 action, char const* code)
	{
		try
		{
			std::string str = std::string(code);
			int nombre_convert = std::stoi(str);
			if (sender == GOSSIP_SENDER_MAIN)
			{
				switch (action)
				{
				case GOSSIP_ACTION_INFO_DEF + 1:

					if (nombre_convert == nombre)
					{
						player->AddItem(323201, 1);
						player->DestroyItemCount(323202, 10, true);
						player->CLOSE_GOSSIP_MENU();
						int nombre = 0;
						isNombre = false;
					}
					else if (nombre > nombre_convert)
					{
						player->GetSession()->SendAreaTriggerMessage("Erreur : c'est plus !");
						player->CLOSE_GOSSIP_MENU();

					}
					else if (nombre < nombre_convert)
					{
						player->GetSession()->SendAreaTriggerMessage("Erreur : c'est moins !");
						player->CLOSE_GOSSIP_MENU();

					}
				}
			}

			return false;

		}

		catch (std::exception e)
		{
			return false;
		}
	}

};


class SystemeAI : public GameObjectScript {


public:
	SystemeAI() : GameObjectScript("SystemeAI"){}

	bool OnGossipHello(Player* player, GameObject* go)
	{
		if (player->GetQuestStatus(200209) == QUEST_STATUS_INCOMPLETE) {

			player->ADD_GOSSIP_ITEM(0, "Mettre a jour l'ordinateur. * |cffFF0000ATTENTION : BUG Bug X-B456 POSSIBLE|r * ", GOSSIP_SENDER_MAIN, 1);
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
				Creature * creature  = player->SummonCreature(75000, player->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 30000);
				creature->AI()->AttackStart(player);
				player->AddItem(250016, 1);
				go->Delete();
			break;
			}

		}

		return true;
	}

};
enum Yells
{
	SAY_AGGRO = 0,
	SAY_SLAY = 1,

};
class boss_Leryssa : public CreatureScript
{
public:
	boss_Leryssa() : CreatureScript("boss_Leryssa") { }

	struct boss_LeryssaAI : public ScriptedAI
	{
		boss_LeryssaAI(Creature* creature) : ScriptedAI(creature) { }

		void EnterCombat(Unit* /*who*/) override
		{
			Talk(SAY_AGGRO);
		}
		void KilledUnit(Unit* victim) override
		{

			if (victim->GetEntry() == 263000)
			{
				victim->SummonGameObject(1615271, victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), 0, 0, 0, 0, 0, 30000);
				me->CastSpell(me, 44290);
				Talk(SAY_SLAY);
			}
		}
		void UpdateAI(uint32 diff) override
		{
			Creature * creature = me->FindNearestCreature(263000, 10.f);
			me->AI()->AttackStart(creature);
			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new boss_LeryssaAI(creature);
	}
};

void AddSC_SymbolixDEV_NPC()
{
	new SymbolixDEV_NPC();
	new Divers_NPC();
	new Gemme_vendor();
	new Professions_NPC();
	new ReliqueUlda();
	new SystemeAI();
	new Bidule();
	new Gard_NPC();
	new Monture_test();
	new Marque_Echanger();
	new set_phase();
	new set_phase_on_die();
	new boss_Leryssa();
}