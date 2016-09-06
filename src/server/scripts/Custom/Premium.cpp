#include "Player.h"
void AddPremium(Player * player, int entry, unsigned int cost)
{
	QueryResult result_level = LoginDatabase.PQuery("SELECT points FROM account WHERE id = %u", player->GetSession()->GetAccountId());
	Field *fields = result_level->Fetch();
	uint64 points = fields[0].GetInt16();
	if (points <= cost)
	{
		std::vector<int>item_give;
		player->GetSession()->SendAreaTriggerMessage("Merci d'avoir acheter le systeme premium Elegon World.");
		item_give.push_back(392020);
		item_give.push_back(392021);
		item_give.push_back(392022);
		item_give.push_back(392023);
		item_give.push_back(392024);
		item_give.push_back(392025);
		item_give.push_back(392026);
		item_give.push_back(392027);
		for (int i(0); i < item_give.size(); i++)
		{
			player->AddItem(item_give[i], 1);
		}
		player->AddItem(entry, 1);
		player->CLOSE_GOSSIP_MENU();
		LoginDatabase.PQuery("UPDATE account SET points = points - %u WHERE id = %u", cost, player->GetSession()->GetAccountId());

	}
	else
	{
		player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de points sur votre compte pour acheter ce systeme premium");
		player->CLOSE_GOSSIP_MENU();

	}

}
class Premium_NPC : public CreatureScript
{
private:
public:
	Premium_NPC() : CreatureScript("Premium_NPC") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|tAcheter Premium : Durée 10 Jours [80 Points Elegon-World]", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|tAcheter Premium : Durée 30 Jours [150 Points Elegon-World]", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|tAcheter Premium : Durée 60 Jours [250 Points Elegon-World]", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_gem_bloodgem_01:30|tAcheter Premium : A vie lié au compte [500 Points Elegon-World]", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Fermer", GOSSIP_SENDER_MAIN, 5);
		player->SEND_GOSSIP_MENU(25500, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				AddPremium(player, 200200, 80);
				break;
			case 2:
				AddPremium(player, 200201, 150);
				break;
			case 3:
				AddPremium(player, 200202, 250);

				break;
			case 4:
				AddPremium(player, 200203, 500);
				break;
			}
		}
		return true;
	}
};
class item_vip_time : public ItemScript
{
public:
	item_vip_time() : ItemScript("item_vip_time") { }

	bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) override
	{
		std::vector<int>item_destroy;
		item_destroy.push_back(392020);
		item_destroy.push_back(392021);
		item_destroy.push_back(392022);
		item_destroy.push_back(392023);
		item_destroy.push_back(392024);
		item_destroy.push_back(392025);
		item_destroy.push_back(392026);
		item_destroy.push_back(392027);

		for (int i(0); i < item_destroy.size(); i++)
		{
			player->DestroyItemCount(item_destroy[i], 1, true);
		}
		return true;
	}
};
class OnQuestCompletion : public CreatureScript
{
public:
	OnQuestCompletion() : CreatureScript("OnQuestCompletion") { }

	bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt)
	{

		if (player->isVIP()){
			switch (urand(1, 2))
			{
			case 1:
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas gagner de bonus de quete supplémentaire désolé !");
				break;
			case 2:
				player->GetSession()->SendAreaTriggerMessage("Vous avez gagner un bonus supplémentaire graçe à votre abonnement premium !");
				player->AddItem(430407, urand(1, 2));
				player->CastSpell(player, 47686);
				break;
			}

		}

		return false;


	}

};

void AddSC_Premium_script()
{
	new Premium_NPC;
	new item_vip_time();
	new OnQuestCompletion();

}