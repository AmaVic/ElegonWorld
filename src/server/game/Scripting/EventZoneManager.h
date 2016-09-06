#include "Group.h"
#include "Player.h"

#ifndef ZONEEVTMGR
#define ZONEEVTMGR

class EventZoneManager {

private :

	Group * grpdef_new;
	Group * grpdef_old;


public : 


	EventZoneManager();
	void AddGroupPlayer(Player * player);
	void RemoveGroup(Player * player);
};

#define sEventZoneMgr ACE_Singleton<EventZoneManager, ACE_Null_Mutex>::instance()
#endif