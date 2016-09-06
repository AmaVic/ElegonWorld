#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include <string>

class  Chest_NPC : public CreatureScript
{
public:
	Chest_NPC() : CreatureScript("Chest_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		creature->HandleEmoteCommand(3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "J'accepte [Cout : Trois marques Defis]", GOSSIP_SENDER_MAIN, 1, "Vous allez etre teleporter dans la salle des coffres de Richard, vous n'aurez que deux minute pour prendre un maximum d'objet.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Non merci", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(36000, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				if (player->HasItemCount(250050, 3)) {
					player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_NORMAL);
					player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false);
					player->GetSession()->SendAreaTriggerMessage("C'est partis, vous n'avez que 2 minutes pour prendre un maximum de coffre !");
					player->TeleportTo(34, 74.660400f, 0.634095f, -25.606199f, 0.000375f);
					player->CLOSE_GOSSIP_MENU();
					player->AddItem(304260, 1);
					player->DestroyItemCount(250050, 3, true);
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de marque de defis");
					player->CLOSE_GOSSIP_MENU();

				}

			break;

			case 2:
				player->CLOSE_GOSSIP_MENU();
				creature->HandleEmoteCommand(3);
				player->CLOSE_GOSSIP_MENU();

			break;

			}

		}
		return true;

	}
};
class  Fast_NPC : public CreatureScript
{
public:
	Fast_NPC() : CreatureScript("Fast_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "J'accepte [Cout : Deux marques Defis]", GOSSIP_SENDER_MAIN, 1, "Vous allez etre transformer en voleur et ceci vous permettra de voler des civils de Theramore. Vous n'aurez que 2 minutes pour voler un maximum de civil. Etes vous pres ?");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Non merci", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(36000, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				player->GetSession()->SendAreaTriggerMessage("C'est partis, vous n'avez que 2 minutes pour voler un maximum de civil de Theramore ! Ne retirer surtout pas votre camouflage, c'est lui qui vous permet de voler des civils !");
				player->AddItem(304260, 1);

				break;

			case 2:
				break;

			}

		}
		return true;

	}
};
class  Rogue_NPC : public CreatureScript
{
public:
	Rogue_NPC() : CreatureScript("Rogue_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		creature->HandleEmoteCommand(3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "J'accepte [Cout : Trois marques Defis]", GOSSIP_SENDER_MAIN, 1, "Vous allez etre transformer en voleur et ceci vous permettra de voler des civils de Theramore. Vous n'aurez que 2 minutes pour voler un maximum de civil. Etes vous pres ?");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Non merci", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(36001, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{

			switch (action) {

			case 1:
				if (player->HasItemCount(250050, 3)) {
					player->TeleportTo(1, -3714.115234f, -4540.350098f, 10.411351f, 3.668732f);
					player->CLOSE_GOSSIP_MENU();
					player->SetDisplayId(2361);
					player->AddAura(58506, player);
					player->SetSpeed(MOVE_RUN, 3, true);
					player->AddItem(304260, 1);
					player->DestroyItemCount(250050, 3, true);
					player->GetSession()->SendAreaTriggerMessage("C'est partis, vous n'avez que 2 minutes pour voler un maximum de civil de Theramore !");
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de marque de defis");
					player->CLOSE_GOSSIP_MENU();

				}

				break;

			case 2:
				player->CLOSE_GOSSIP_MENU();
				creature->HandleEmoteCommand(3);
				break;

			}

		}
		return true;

	}
};
class item_expire : public ItemScript
{
public:
	item_expire() : ItemScript("item_expire") { }

	bool OnExpire(Player* player, ItemTemplate const* proto)
	{

		player->SetSpeed(MOVE_RUN, 1, true);
		player->GetSession()->SendAreaTriggerMessage("l'Event est maintenant termine.");
		player->DeMorph();
		player->TeleportTo(1, -3714.115234f, -4540.350098f, 10.411351f, 3.668732f);
		player->DestroyItemCount(304260, 1, true);
		player->RemoveAurasDueToSpell(58506, player->GetGUID());
		return true;
	
	}

};
void AddSC_TheramorNPC()
{
	new Chest_NPC();
	new Rogue_NPC();
	new Fast_NPC();
	new item_expire();
}