

#include "ScriptMgr.h"

class script_cmd_income : public CommandScript
{

public: //Constructeur

	script_cmd_income() : CommandScript("script_cmd_income")
	{

	}

public:

	static bool HandleAccepterCommand(ChatHandler* handler, const char* /*args*/)
	{
		handler->PSendSysMessage("Hello World");
		return true;
	}

	static bool HandleRefuseCommand(ChatHandler* handler, const char* /*args*/)
	{
		handler->PSendSysMessage("Hello World");
		return true;
	}

	ChatCommand* GetCommands() const
	{
		static ChatCommand commandTable[] =
		{
			{ NULL, 0, false, NULL, "", NULL }
		};
		return commandTable;
	}

};

void AddSC_script_cmd_income()
{
	new script_cmd_income();
}