#include "EventZoneManager.h"
#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "Group.h"
#include <iostream>
#include "Vehicle.h"
#include "EventZoneManager.h"

#define ITEM_ID 3000

class OnPlayerEnterGroupEvent : public PlayerScript
{
private :

uint32 _lastAreaid;

public:
OnPlayerEnterGroupEvent() : PlayerScript("OnPlayerEnterGroupEvent") {}

void OnUpdateZone(Player * player, uint32 newZone, uint32 newArea) {

WorldPacket data;
ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, Language(0), player, player, "|cff00ff00 Bienvenue au Soltice, vous avez 60 minutes pour terminer tous les quetes en cours. Mettez vous en groupes avec vos compagnons pour faire les quetes plus facilement.");
player->SendMessageToSetInRange(&data, 0, true);
}
};


class npc_reward_quest : public CreatureScript
{
public:
	npc_reward_quest() : CreatureScript("npc_reward_quest") { }

	bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt)

	{
		if (player->GetAreaId() == 4205) {
			CharacterDatabase.PQuery("UPDATE event_custom_player SET points = points +1 WHERE guid = %u", player->GetGUID());
			player->GetSession()->SendAreaTriggerMessage("Vous recevez 1 point");
		}

		return true;
	}

	bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)

	{
		if (quest->GetQuestId() == 250000) {
			creature->MonsterWhisper("Transporter 250 stocks de ressources à la tour en bois au Nord-Ouest d'ici. Vous verrez c'est pas si dur que ca et puis de toute facon vous avez pas le choix !", player);
			creature->HandleEmoteCommand(1);

		}
		if (quest->GetQuestId() == 250001) {
			creature->MonsterWhisper("Merci de m'aider $n,  vous trouverez ce que vous avez besoin sur les Rodeurs Hurlesoir. Ils se trouvent tous au tour du campement. ", player);
			creature->HandleEmoteCommand(1);

		}
		if (quest->GetQuestId() == 250002) {
			creature->MonsterWhisper("Merci de m'aider $n, vous trouverez les planches de bois aux pieds des arbres. Cela ne devrait pas être difficile de trouver un arbre", player);
			creature->HandleEmoteCommand(1);

		}
		if (quest->GetQuestId() == 250003) {
			creature->MonsterWhisper("Vous trouverez les bottes de foin sur les hautes tours en bois dans le campement, faite vite $n.", player);
			creature->HandleEmoteCommand(1);

		}
		if (quest->GetQuestId() == 250004) {
			creature->MonsterWhisper("Merci de m'aider $n, vous trouverez au lac de grisgueule des poissons et des cerfs j'ai besoin de 250 viandes de cerf et 250 poissons !", player);
			creature->HandleEmoteCommand(1);

		}
		if (quest->GetQuestId() == 250005) {
			creature->MonsterWhisper("Activez vous $n ! l'enemis est a nos portes.", player);
			creature->HandleEmoteCommand(5);

		}
		if (quest->GetQuestId() == 250006) {
			creature->MonsterWhisper("Le temps presse soldat $n, vous et vos compagnons devez abattre ce monstre de guerre. Il est juste devant notre porte ! ", player);
			creature->HandleEmoteCommand(5);
		}
		if (quest->GetQuestId() == 250007) {
			creature->MonsterWhisper("Le temps n'est pas plus la democratie soldat, l'enemis est juste a notre porte à l'Est.", player);
			creature->HandleEmoteCommand(5);
		}

		if (quest->GetQuestId() == 200212) {
			
			Creature* go = player->SummonCreature(33062, player->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 360000);
			if (!go->IsVehicle())
				return false;
			Vehicle* v = go->GetVehicleKit();
			if (!v)
				return false;

			player->EnterVehicle(v->GetBase(), 0);

		}
		return true;
	}

};
class Stock_gob : public GameObjectScript {


public:
	Stock_gob() : GameObjectScript("Stock_gob"){}

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
				player->AddItem(430400, 1);
			break;
			}

		}

		return true;
	}

};
class livre_gob : public GameObjectScript {


public:
	livre_gob() : GameObjectScript("livre_gob"){}

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
				if (player->HasItemCount(62831, 1, true)) { return true; }
				else if (player->HasItemCount(62830, 1, true)) {
					player->AddItem(62831, 1);
					CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(206);
					player->SetTitle(titleInfo);
				}
				break;
			}

		}

		return true;
	}

};
class OnplayerLogoff : public PlayerScript
{
public:

	OnplayerLogoff() : PlayerScript("OnplayerLogoff") {}

	void OnLogout(Player* player) {

		player->TeleportTo(1, -3731.560059f, -4549.109863f, 27.127600f, 0.436029f);
		player->RemoveFromGroup();
		}
};
void AddSC_ZoneEventScript()
{
	new npc_reward_quest();
	new Stock_gob();
	new OnplayerLogoff();
	new livre_gob();
}