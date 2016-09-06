
#include "ScriptMgr.h"
#include "RiftHandler.h"

class script_open_rift : public PlayerScript
{

public:

	script_open_rift() : PlayerScript("script_open_rift") { }

	void OnUpdateZone(Player* player, uint32 /*newZone*/, uint32 newArea) {
		if (sRH->IsOpen() && !sRH->IsSummoned() && newArea == sRH->GetAreaId())
		{
			sRH->OnPlayerGettingClose();
		}
	}

};


class script_run_rift : public WorldMapScript
{
public:

	script_run_rift(uint32 mapId) : WorldMapScript("script_run_rift", mapId){}

public:

	void OnUpdate(Map* map, uint32 /*diff*/)
	{



		//Si une faille est ouverte, on ne fait rien
		if (sRH->IsOpen())
		{
			return;
		}


		//Si aucun joueur dans cette map, on ne fait rien
		if (map->GetPlayers().isEmpty())
			return;

		//Sinon, on ouvre une nouvelle faille
		auto it = map->GetPlayers().begin();
		if (it == map->GetPlayers().end())
			return;

		sRH->OpenNewRift(it->GetSource(), sRH->GetLastCreatureGUID());
	}
};

void AddSC_script_run_rift()
{
	new script_run_rift(0);
	new script_run_rift(1);
	new script_run_rift(571);
	new script_open_rift();
}