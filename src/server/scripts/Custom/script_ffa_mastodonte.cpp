#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Group.h"
#include <ctime>
#include "GroupMgr.h"
#include "Player.h"
#include "MastoHandler.h"


class script_removeMastoOnPlayerLeave : public PlayerScript
{
public:
	
	script_removeMastoOnPlayerLeave() : PlayerScript("script_removeMastoOnPlayerLeave")
	{

	}

	void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 newArea)
	{
		if (newArea != 720 && sMastoHandler->isMasto(player->GetGUID()))
		{
			sMastoHandler->choseNewMastoAndKeepEventGoing();
		}

	}

	void OnLogout(Player* player)
	{
		if (sMastoHandler->isMasto(player->GetGUID()))
			sMastoHandler->choseNewMastoAndKeepEventGoing();
	}


};

class script_onMastoKilled : public PlayerScript
{
public:

	script_onMastoKilled() : PlayerScript("script_onMastoKilled"){}

public:

	void OnPVPKill(Player* killer, Player* killed)
	{
		if (!sMastoHandler->eventIsStarted())
			return;

		if (sMastoHandler->isMasto(killed->GetGUID()))
		{
			sMastoHandler->onMastoKilled();
		}
	}


};


void AddSC_script_ffa_mastodonte()
{
	new script_removeMastoOnPlayerLeave();
	new script_onMastoKilled();
}

