#ifndef RS
#define RS

#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "Group.h"

#include <map>
#include <array>

enum RaidSize
{
	RAID_2  = 2,
	RAID_8	= 8,
	RAID_10 = 10,
	RAID_25 = 25
};

#define TANK_INDEX	 0
#define HEALER_INDEX     1
#define DPS_INDEX	 2

#define TIME_TO_VOTE     30000 //30sec

#define ZG_MAP_ID        1
#define ZG_X             -1671.438599f
#define ZG_Y             -4317.183594f
#define ZG_Z             3.359515f
#define ZG_O             0.933025f

#define COLOR_GREEN  "|cff00ff00"
#define COLOR_RED    "|cffff0000"
#define COLOR_YELLOW "|cffffff00"

#define RGM_AURA 0

template<RaidSize RAID_SIZE>
class RGM
{
	//OVERVIEW: RGM = RaidGroupMaker -> permet de cr�er des groupes de joueurs pour partir en raid
	//Mutable

	//Peut (actuellement) g�rer un raid par taille de groupe;
	//Exemple: un raid 10: ZG
	//         un dongeon 5: Mortes mines
	//         un raid 25: ICC
	//         mais pas un second raid10/raid25/dongeon

	//=> todo: impl�menter pour diff�rentes instances de m�me taille de groupe
	//   todo: donner la possibilit� de s'inscrire en choisissant plusieurs r�les

public: 

	//--------------------------------------------
	//Enums
	//--------------------------------------------
	enum PlayerRole : uint32
	{
		ROLE_TANK = 0,
		ROLE_HEAL = 1,
		ROLE_DPS  = 2
	};

public: 

	//--------------------------------------------
	//Typedefs
	//--------------------------------------------
	typedef uint64							 PlayerGUID;
	typedef std::map<PlayerGUID, PlayerRole> RGMPlayers;
	typedef std::array<int, 3 /*3 roles*/>   RGMComposition; //Premier elem = #tanks; 2�: #healers; 3�: #dps

public:

	//--------------------------------------------
	//Inner struct
	//--------------------------------------------
	struct RGMProposal
	{
		//OVERVIEW: une RGMProposal est une proposition faite � un joueur de rejoindre un groupe pour partir en raid
		//Mutable

		//---------------------------

		//Repr�sentation:
		bool   inProgress;
		bool   answer;
		uint32 elapsedTimeSinceProposal;
	};

public:

	//--------------------------------------------
	//Constructeur
	//--------------------------------------------
	RGM();

public:

	//--------------------------------------------
	//M�thodes publiques
	//--------------------------------------------
	void addPlayerInQueue(PlayerGUID guid, PlayerRole playerRole);
		//REQUIRES: guid est un GUID valide
		//MODIFIES: this
		//EFFECTS: le joueur ayant pour GUID: guid est maintenant dans la file d'attente et a choisi playeRole comme role
		//S'il est possible de faire un groupe, on envoie aux joueurs les propositions

	bool isPlayerInQueue(PlayerGUID guid) const;
		//EFFECTS: retourne true si le joueur ayant pour GUID guid est inscrit dans la recherche

	bool isPlayerSelected(PlayerGUID guid) const;
		//EFFECTS: retourne true si le joueur est s�lectionn� pour un raid

	void removePlayerFromQueue(PlayerGUID guid);
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: le joueur ayant pour GUID: guid n'est plus dans la file d'attente
	
	void onUpdate(uint32 diff);
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: met � jour les RGMProposal des joueurs (timing)


private:

	//--------------------------------------------
	//M�thodes priv�es
	//--------------------------------------------
	void sendMessageToPlayer(PlayerGUID guid, const char* color, const char* msg) const;
		//REQUIRES: guid est un GUID valide
		//MODIFIES: /
		//EFFECTS: envoie le message msg au joueur (affiche le message dans le chat)

	std::string playerRoleToString(PlayerRole role) const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne un string contenant le PlayerRole role

	void updateRoleCount();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: le nombre de tanks,heals, dps dans la recherche est mis � jour

	void sendProposalToPlayer(PlayerGUID guid) const;
		//REQUIRES: guid est un GUID valide
		//MODIFIES: le joueur ayant pour GUID guid
		//EFFECTS: Envoie au joueur ayant guid comme GUID une proposition pour rejoindre un groupe de raid

	bool groupCanBeMade() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne true s'il est possible de former un groupe; false sinon

	void chosePlayersToInvite();
		//REQUIRES: Il y a au moins RAID_SIZE joueurs dans la file d'attente && groupCanBeMade() == true
		//MODIFIES: this	
		//EFFECTS: les joueurs s�lectionn�s font partie des joueurs � inviter
	
	void sendProposals();
		//REQUIRES:  chosePlayersToInvite a �t� appel�e
		//MODIFIES: this
		//EFFECTS: Appelle sendProposalToPlayer pour chacun des joueurs s�lectionn�s par chosePlayersToInvite() et dit que les 				   propositions ont �t� envoy�es

	int getAcceptInviteCount() const;
		//REQUIRES: Les invitations ont �t� envoy�es
		//MODIFIES: /
		//EFFECTS: retourne le nombre de joueurs ayant re�u une proposition et l'ayant accept�e

	void resetPlayerProposal(PlayerGUID guid) const;
		//REQUIRES: /
		//MODIFIES: le joueur ayant pour GUID guid
		//EFFECTS: La proposition faite au joueur ayant guid comme GUID est r�initialis�e (comme s'il n'y en avait pas)

private:

	//--------------------------------------------
	//Repr�sentation
	//--------------------------------------------
	RGMPlayers mQueue;
	std::array<PlayerGUID, RAID_SIZE> mPlayersToInvite; 
	std::array<PlayerGUID, RAID_SIZE> mPlayersToGroupAndTeleport; //le GUID de ces joueurs
	int mTankCount;
	int mHealCount;
	int mDpsCount;
	RGMComposition mCompo;
	uint32 mTimeSinceProposal;
	bool mProposalSent;
	

};

#include "RGM.inl"

#define sRGM2  ACE_Singleton<RGM<RAID_2>,  ACE_Null_Mutex>::instance()
#define sRGM8  ACE_Singleton<RGM<RAID_8>,  ACE_Null_Mutex>::instance()
#define sRGM10 ACE_Singleton<RGM<RAID_10>, ACE_Null_Mutex>::instance()
#define sRGM25 ACE_Singleton<RGM<RAID_25>, ACE_Null_Mutex>::instance()


#endif
