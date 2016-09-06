

#include "ScriptMgr.h"
#include "RGM.h"
#include <string>

using namespace std;

class script_command_debug_raid_group_maker : public CommandScript
{

public: //Constructeur

	script_command_debug_raid_group_maker() : CommandScript("script_command_debug_raid_group_maker"){}


public: //Override

	ChatCommand* GetCommands() const override
	{
		static ChatCommand raidCommands[] = 
		{
			{ "oui", rbac::RBAC_PERM_COMMAND_OUI, false, &HandleOuiCommand, "", NULL },
			{ "non", rbac::RBAC_PERM_COMMAND_NON, false, &HandleNonCommand, "", NULL },
			{ "status", rbac::RBAC_PERM_COMMAND_NON, false, &HandleStatusCommand, "", NULL },
			{ NULL, 0, false, NULL }
		};

		static ChatCommand CommandTable[] =
		{
			{ "raid", rbac::RBAC_PERM_COMMAND_OUI, false, NULL, "", raidCommands },
			{ NULL, 0, false, NULL }
		};

		return CommandTable;
	}

	static bool HandleOuiCommand(ChatHandler* handler, char const* args)
	{
		if (!sRGM2->isPlayerInQueue(handler->GetSession()->GetPlayer()->GetGUID()))
		{
			handler->GetSession()->SendAreaTriggerMessage("Vous n'etes pas inscrit en recherche de raid");
			return true;
		}

		if (!sRGM2->isPlayerSelected(handler->GetSession()->GetPlayer()->GetGUID()))
		{
			handler->GetSession()->SendAreaTriggerMessage("Nous recherchons toujours des joueurs...");
			return true;
		}

		handler->GetSession()->GetPlayer()->getProposal().answer = true;
		handler->GetSession()->SendAreaTriggerMessage("En attente des autres joueurs ...");
		handler->GetSession()->SendAreaTriggerMessage("Vous avez accepte l'invitation de recherche de raid");

		return true;
	}

	static bool HandleNonCommand(ChatHandler* handler, char const* args)
	{
		if (!sRGM2->isPlayerInQueue(handler->GetSession()->GetPlayer()->GetGUID()))
		{
			handler->GetSession()->SendAreaTriggerMessage("Vous n'etes pas inscrit en recherche de raid");
			return true;
		}

		if (!sRGM2->isPlayerSelected(handler->GetSession()->GetPlayer()->GetGUID()))
		{
			handler->GetSession()->SendAreaTriggerMessage("Nous recherchons toujours des joueurs...");
			return true;
		}

		handler->GetSession()->SendNotification("Vous avez refuse l'invitation de recherche de raid");
		handler->GetSession()->GetPlayer()->getProposal().answer = false;

		return true;
	}

	static bool HandleStatusCommand(ChatHandler* handler, char const* args)
	{
		return true;
	}
};

void AddSC_script_debug_cmd_raid()
{
	new script_command_debug_raid_group_maker();
}