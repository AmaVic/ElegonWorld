
#include "ScriptMgr.h"
#include "SponsorshipHandler.h"

#define GOSSIP_SUBSCRIBE_SPONSORSHIP "Parrainer quelqu'un"
#define GOSSIP_TYPE_PLAYER_NAME "Cliquez sur accepter et ensuite entrez le nom d'un des joueur du compte que vous souhaitez parrainer"
enum
{
	ACTION_TYPE_PLAYER_NAME,
	ACTION_CHECK_IF_SPONSORSHIP_OK
};

class script_creature_parrainage : public CreatureScript
{

public: //Constructeur

	script_creature_parrainage() : CreatureScript("script_crea_parrainage"){}

public: //Overriding

	virtual bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM_EXTENDED(0, GOSSIP_SUBSCRIBE_SPONSORSHIP, GOSSIP_SENDER_MAIN, ACTION_CHECK_IF_SPONSORSHIP_OK, GOSSIP_TYPE_PLAYER_NAME, 0, true);

		player->SEND_GOSSIP_MENU(56500, creature->GetGUID());

		return true;
	}

	virtual bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
		switch (action)
		{

		case ACTION_CHECK_IF_SPONSORSHIP_OK:
		{

											   player->PlayerTalkClass->ClearMenus();

											   //Si le joueur essaye de se parrainer lui-même: interdit
											   if (code == player->GetName().c_str())
											   {
												   player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas vous parrainer vous-meme");
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }

						
											   //Si le nom entré n'existe pas, on le dit au joueur et on ferme le gossip
											   if (!SponsorshipHandler::playerExists(code))
											   {
												   player->GetSession()->SendAreaTriggerMessage("Aucun joueur en ligne ne s'appelle %s", code);
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }

											   //Si le nom existe, on recherche l'id du compte du parrainé et on récupère celle du parrain
											   uint32 sponsorAccountId = player->GetSession()->GetAccountId();
											   uint32 sponsoredAccountId = SponsorshipHandler::findAccountId(code);


											   //On regarde si les joueur n'est pas déjà en relation de parrainage (pas forcément entre eux)
											   if (SponsorshipHandler::isInSponsorship(sponsorAccountId) || SponsorshipHandler::isInSponsorship(sponsoredAccountId))
											   {
												   player->GetSession()->SendAreaTriggerMessage("Vous etes deja en relation de parrainage ou alors c'est le cas pour %s", code);
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }

											  
											   //On regarde si l'association des comptes est possible
											   //Si ce n'est pas le cas, on le dit au joueur et on ferme le gossip
											   if (!SponsorshipHandler::sponsoringPossible(sponsorAccountId, sponsoredAccountId))
											   {
												   player->GetSession()->SendAreaTriggerMessage("Parrainage impossible: soit vous n'avez pas encore 24h de jeu sur Elegon World, soit la personne que vous voulez parrainer a la meme adresse IP que vous", code);
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }

											   //Si tout est ok, on regarde si les joueurs ne sont pas déjà parrainés entre eux et on essaye de faire le lien de parrainage
											   if (!SponsorshipHandler::registerSponsorship(sponsorAccountId, sponsoredAccountId))
											   {
												   player->GetSession()->SendAreaTriggerMessage("Vous parrainez deja le compte de %s", code);
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }

											   player->GetSession()->SendAreaTriggerMessage("Vous etes parrain du compte du %s pendant 30 jours !", code);
											   Player* sponsored = sObjectAccessor->FindPlayerByName(code);
											   if (!sponsored)
											   {
												   player->CLOSE_GOSSIP_MENU();
												   return true;
											   }
											   sponsored->GetSession()->SendAreaTriggerMessage("%s parraine votre compte pour 30j", player->GetName().c_str());
											   player->CLOSE_GOSSIP_MENU();
											   return true;
		}
			break;
		}
		}

		return true;
	}
};

void AddSC_script_parrainage()
{
	new script_creature_parrainage();
}