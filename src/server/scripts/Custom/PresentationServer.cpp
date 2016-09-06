/*
<--------------------------------------------------------------------------->
- Developer(s): Mthsena
- Complete: %100
- ScriptName: 'PresentationOfTheServer'
- Comment: Tested.
- Shared for:Emudevs by Synth
<--------------------------------------------------------------------------->
*/
 
#include "ScriptPCH.h"
 
enum Enums
{
        FIRST_TELEDELAY    = 10000,
        SECOND_TELEDELAY   = 20000,
        THIRD_TELEDELAY    = 40000,
        FOURTH_TELEDELAY   = 55000,
	    FIVE_TELEDELAY     = 65000,
		SIXE_TELEDELAY     = 85000,
		SEVEN_TELEDELAY    = 105000,
		HEIGHT_TELEDELAY   = 125000,
		NINE_TELEDELAY     = 130000,
        BACK_TELEDELAY     = 140000,
};
 
struct Event
{
        uint8 Events;
};
static std::map<uint32, Event> _events;
 
class Teleport : public BasicEvent
{
public:
        Teleport(Player* player) : _Plr(player) {}
 
        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
                uint32 pEvent;
                pEvent = _Plr->GetGUID();
 
                switch(_events[pEvent].Events)
                {
                case 0: // First Teleport
					_Plr->TeleportTo(1, -3710.832520f, -4538.634766f, 25.833887f, 0.4f); // Teleport Coordenates
                        _Plr->GetSession()->SendAreaTriggerMessage("Voici l'equipement de depart. A gauche se trouvent les fournitures et les competances d'armes et a droite les glyphes."); // Depart
                        _events[pEvent].Events = 1;
                        break;
 
                case 1: // Second Teleport
					_Plr->TeleportTo(1, -3736.0f, -4556.0f, 11.910501f, 5.0f);
						_Plr->GetSession()->SendAreaTriggerMessage("Voici la zone des recompenses de defis. Venez y faire un tour quand vous aurez des marques defis."); // Description, Tip
						_events[pEvent].Events = 2;
                        break;
 
                case 2: // Third Teleport
					_Plr->TeleportTo(1, -3728.05f, -4572.260254f, 11.910501f, 3.6f);
						_Plr->GetSession()->SendAreaTriggerMessage("Voici le palier 2. Il s'obtient dans les donjons d'Elegon World en mode normal uniquement."); // Palier 2
						_events[pEvent].Events = 3;
                        break;
 
                case 3: // Fourth Teleport
					_Plr->TeleportTo(1, -3711.71f, -4514.27f, 11.910f, 0.489943f);
					_Plr->GetSession()->SendAreaTriggerMessage("Voici le palier 3. Il s'obtient dans les donjons d'Elegon World en mode heroique uniquement."); // Palier 2
					_events[pEvent].Events = 4;
                        break;
 
				case 4:
					_Plr->TeleportTo(1, -3748.946289f, -4335.004883f, 10.177468f, 0.002501f);
					_Plr->GetSession()->SendAreaTriggerMessage("Voici la premiere saison JcJ d'Elegon World. Elle s'obtient en echange d'honneur et de points d'arene."); // Description, Tip
					_events[pEvent].Events = 5;
					break;
				case 5:
					_Plr->TeleportTo(1, -3685.766846f, -4506.187500f, 11.947780f, 3.151952f);
					_Plr->GetSession()->SendAreaTriggerMessage("Vous pouvez rejoindre le BG League of Elegon grace au PNJ devant vous ou par l'interface en appuyant sur la touche <h>."); // Palier 2
					_events[pEvent].Events = 6;
					break;
				case 6:
					_Plr->TeleportTo(1, -3837.564206f, -4452.482910f, 11.337552f, 0.435244f);
					_Plr->GetSession()->SendAreaTriggerMessage("Voici la boutique d'Elegon World. Plus de details sur la page decouverte du site web."); // Palier 2
					_events[pEvent].Events = 7;
					break;
				case 7:
					_Plr->TeleportTo(1, -3624.044434f, -4448.898438f, 14.247352f, 4.684493f);
					_Plr->GetSession()->SendAreaTriggerMessage("Voici l'auberge d'Elegon world, venez y faire un tour quand vous avez temps a perdre !"); // Palier 2
					_events[pEvent].Events = 8;
					break;
                case 8: // Back Teleport
					_Plr->TeleportTo(1, -3730.962646f, -4548.962402f, 27.128534f, 0.457401f);
					_Plr->GetSession()->SendAreaTriggerMessage("La presentation est maintenant terminee, pour voyager dans le monde d'Elegon World, utilisez votre pierre de transposition.");
					_Plr->AddItem(456500, 10);
					_events.clear();
					_events[pEvent].Events = 9;
                        break;
                }
                return true;
        }
        Player* _Plr;
};
class PresentationOfTheServer : public CreatureScript {


public:
	PresentationOfTheServer() : CreatureScript("PresentationOfTheServer"){}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(0, "Me faire visiter le serveur. (10 Marques Boutique en bonus a la fin !)", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, "Non merci, je vais regarder par moi meme.", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Installez vous confortablement, nous allons vous faire faire un tour de l'ile de Theramore. Ceci ne prendra que quelques minutes..."); // Initial message before starting...
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(FIRST_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(SECOND_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(THIRD_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(FOURTH_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(FIVE_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(SIXE_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(SEVEN_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(HEIGHT_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(NINE_TELEDELAY));
				player->m_Events.AddEvent(new Teleport(player), player->m_Events.CalculateTime(BACK_TELEDELAY));
			break;
			case 2:
				player->CLOSE_GOSSIP_MENU();
				break;
			}

		}

		return true;
	}

};
void AddSC_PresentationOfTheServer()
{
  new PresentationOfTheServer;
}