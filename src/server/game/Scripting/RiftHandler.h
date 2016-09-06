
#ifndef DEF_RH
#define DEF_RH

#include "Group.h"
#include "Player.h"
#include "GroupMgr.h"
#include "World.h"
#include "DBCStores.h"
#include "DBCStructure.h"
#include "WorldSession.h"
#include "Player.h"

#include <array>
#include <tuple>


#define MAX_LOCATIONS 3
#define TEXT_NEW_RIFT 300000
#define RIFT_AURA_NPC 1500001
#define RIFT_AURA 72405

#define ITEM_ON_RIFT_CLOSED 10001
#define ITEM_COUNT 5

class RiftHandler
{

public:

	//-----------------------------------------------
	//Constructeur
	//-----------------------------------------------
	RiftHandler();

public:

	//-----------------------------------------------
	//Méthodes publiques
	//-----------------------------------------------
	void OpenNewRift(Player* player, uint64 creatureGUID);
		//REQUIRES: creatureGUID in CREATURES_GUID
		//MODIFIES: this
		//EFFECTS: Une nouvelle faille est ouverte à une position différente de la précédente
		//     &&  Les joueurs sont avertis de la position de la faille

	uint64 GetLastCreatureGUID() const;
	bool   IsOpen() const;
	bool IsUnderAttack() const;


	void SetUnderAttack(bool ua);
	std::string GetAreaName() const;
	std::string GetZoneName() const;

	//-----------------------------------------------
	//OnEvent
	//-----------------------------------------------

	void OnRiftClosed(uint64 creatureGUID);
		//REQUIRES: creatureGUID in CREATURES_GUID
		//MODIFIES: this
		//EFFECTS: récompense les joueurs ayant fermé la faille
		//      && ouvre une nouvelle faille ailleurs
	    //      && dissout le groupe

	void OnPlayerGettingClose();
		//REQUIRES: player != nullptr
		//MODIFIES: player, this
		//EFFECTS: Le joueur player est ajouté au groupe pour fermer la faille si le groupe existe déjà
		//         Si le groupe n'existait pas, il est créé et le joueur en est le leader
		//         Si le joueur était déjà groupé, il est dégroupé

	void OnPlayerGettingAway(Player* player);
		//REQUIRES: player != nullptr && player est dans le groupe pour la faille
		//MODIFIES: player, this
		//EFFECTS: Supprime le joueur du groupe pour la fermeture de la faille

	//-----------------------------------------------
	//Accesseurs
	//-----------------------------------------------
	

	Position GetRiftPosition() const;
	uint32   GetAreaId() const;
	bool     IsSummoned() const;

private:

	//-----------------------------------------------
	//Méthodes privées
	//-----------------------------------------------

	void RewardPlayers();
		//REQUIRES: le groupe n'a pas encore été dissout
		//MODIFIES: les players dans le groupe pour fermer la faille
		//EFFECTS: récompense les joueurs dans le groupe pour la fermeture de la faille

	uint64 ChoseRandomCreatureGUID(uint64 lastCreatureGUID) const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: choisit aléatoirement une nouvelle créature sur laquelle faire spawn la faille
		// &&      le créature aura un GUID différent de lastCreatureGUID



private:

	//-----------------------------------------------
	//Représentation
	//-----------------------------------------------
	WorldLocation									 mCurrentRiftLocation;
	bool											 mIsUnderAttack;
	std::map<uint64/*CREA_GUID*/, uint32/*AREA_ID*/> mData;
	uint64											 mLastCreatureGUID;
	bool											 mIsOpen;
	Creature*                                        mLastCreature;
	std::string										 mAreaName;
	std::string										 mZoneName;
	uint32										     mPlayersInGroup;
	TempSummon *									 mTs;
	uint32											 mAreaId;
	bool										     mIsSummoned;
	

};


#define sRH ACE_Singleton<RiftHandler, ACE_Null_Mutex>::instance()

#endif