#include "ScriptMgr.h"
#include "RGM.h"

class script_rgm_decoplayer : public PlayerScript
{

public: //Constructeur

	script_rgm_decoplayer() : PlayerScript("script_rgm_decoplayer"){}

public: //Override

	//Suppression du joueur de la recherche de raid lors de la déconnexion
	void OnLogout(Player* player)
	{
		sRGM2->removePlayerFromQueue(player->GetGUID());
		sRGM8->removePlayerFromQueue(player->GetGUID());
		sRGM10->removePlayerFromQueue(player->GetGUID());
		sRGM25->removePlayerFromQueue(player->GetGUID());
	}

};

void AddSC_script_rgm_decoplayer()
{
	new script_rgm_decoplayer();
}