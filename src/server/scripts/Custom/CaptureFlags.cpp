#include <vector>

#define HORDE_FACTION 85
#define ALLIANCE_FACTION 11

void capture_flags(Player * player, GameObject *go)// Fonction to capture the flags
{
	int _faction = player->GetTeam();

	if (_faction == HORDE)
	{
		if (sGameEventMgr->IsActiveEvent(70)) // Si la horde le détiens déjà il ne se passe rien
		{
			player->GetSession()->SendAreaTriggerMessage("Votre faction dispose deja de ce drapeau !");
			player->CLOSE_GOSSIP_MENU();
		}
		else
		{
			player->SetHonorPoints(player->GetHonorPoints() + 10000);
			go->CastSpell(player, 34656);
			sGameEventMgr->StartEvent(70); // Sinon on active l'Event Horde
			sGameEventMgr->StopEvent(71); // On stop l'Event Alliance
			sGameEventMgr->StopEvent(72); // On stop l'Event neutre
		}

	}
	if (_faction == ALLIANCE)
	{
		if (sGameEventMgr->IsActiveEvent(71)) // Si l'alliance le détiens déjà il ne se passe rien
		{
			player->GetSession()->SendAreaTriggerMessage("Votre faction dispose deja de ce drapeau !");
			player->CLOSE_GOSSIP_MENU();
		}
		else
		{
			player->SetHonorPoints(player->GetHonorPoints() + 10000);
			go->CastSpell(player, 34656);
			sGameEventMgr->StartEvent(71); // Sinon on active l'Event Alliance
			sGameEventMgr->StopEvent(70); // On stop l'Event la horde
			sGameEventMgr->StopEvent(72); // On stop l'Event neutre
		}
	}
}


class Flags_capture_neutral : public GameObjectScript { /* Possibilité de capture le drapeau */

private :

	uint8 last_guid_alliance;
	uint8 last_guid_horde;

public:
	Flags_capture_neutral() : GameObjectScript("Flags_capture_neutral"){}

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
				if (player->GetTeam() == HORDE)
				{
					if (last_guid_horde == player->GetGUID())
					{
						player->GetSession()->SendAreaTriggerMessage("Vous avez deja pris ce drapeau !");
						player->CLOSE_GOSSIP_MENU();
						return false;
					}
					else
					{
						capture_flags(player, go);
						last_guid_horde = player->GetGUID();

					}
				}
				if (player->GetTeam() == ALLIANCE)
				{
					if (last_guid_alliance == player->GetGUID())
					{
						player->GetSession()->SendAreaTriggerMessage("Vous avez deja pris ce drapeau !");
						player->CLOSE_GOSSIP_MENU();
						return false;
					}
					else
					{
						capture_flags(player, go);
						last_guid_alliance = player->GetGUID();

					}
				}
				break;

			}
		}
		return true;

	}

};

void AddSC_capture_flags_script()
{
	new Flags_capture_neutral();
}