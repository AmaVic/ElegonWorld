
#include "ScriptMgr.h"
#include "RiftHandler.h"

class script_debug_rift : public CommandScript
{

public: //Constructeur

	script_debug_rift() : CommandScript("script_debug_rift"){}


public: //Override

	ChatCommand* GetCommands() const override
	{

		static ChatCommand debugCommandTable[] =
		{
			{ "rift", rbac::RBAC_PERM_COMMAND_START_RIFT, false, &HandleStartRiftCommand, "", NULL },
			{ "closerift", rbac::RBAC_PERM_COMMAND_CLOSE_RIFT, false, &HandleCloseRiftCommand, "", NULL },
			{ NULL, 0, false, NULL }
		};

		return debugCommandTable;

	}

	static bool HandleStartRiftCommand(ChatHandler* handler, char const* args)
	{
		//sRH->OpenNewRift(handler->GetSession()->GetPlayer(), 0);
		return true;
	}

	static bool HandleCloseRiftCommand(ChatHandler* handler, char const* args)
	{
		sRH->OnRiftClosed(sRH->GetLastCreatureGUID());
		return true;
	}

};

void AddSC_script_debug_rift()
{
	new script_debug_rift();
}