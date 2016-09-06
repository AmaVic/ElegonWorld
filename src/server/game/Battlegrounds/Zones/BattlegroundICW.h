#ifndef __BattlegroundICW_H
#define __BattlegroundICW_H

#include "Battleground.h"

#define BG_ICW_TIME_LIMIT          (1*MINUTE*IN_MILLISECONDS);
#define TIME_BETWEEN_MINION_WAVES  30000 //30s entre l'apparition des vagues de minions

//Alliance:
//Droite
#define MINION_POS_A_1_X		   -939.722107f
#define MINION_POS_A_1_Y		   -1212.768066f
#define MINION_POS_A_1_Z		   -29.636930f
#define MINION_POS_A_1_O			0.003485f

//Gauche
#define MINION_POS_A_2_X		   -977.078735f
#define MINION_POS_A_2_Y		   -1168.936401f
#define MINION_POS_A_2_Z		   -28.907537f
#define MINION_POS_A_2_O			1.461240f

//Centre
#define MINION_POS_A_3_X		   -983.991882f
#define MINION_POS_A_3_Y		   -1215.907227f
#define MINION_POS_A_3_Z		   -22.501015f
#define MINION_POS_A_3_O			0.713686f

//Horde:
//Droite:
#define MINION_POS_H_1_X			-763.498352f
#define MINION_POS_H_1_Y			-983.609070f
#define MINION_POS_H_1_Z			-29.636930f
#define MINION_POS_H_1_O			3.168971f

//Gauche:
#define MINION_POS_H_2_X			-723.238342f
#define MINION_POS_H_2_Y			-1023.774902f
#define MINION_POS_H_2_Z			-29.072250f
#define MINION_POS_H_2_O			4.727983f

//Centre:
#define MINION_POS_H_3_X			-723.008728f
#define MINION_POS_H_3_Y			-989.737183f
#define MINION_POS_H_3_Z			-22.493143f
#define MINION_POS_H_3_O		     3.887607f


#define UNITS_SPEED                  1.5f

#define FACTION_ALLIANCE             1078
#define FACTION_HORDE                85

#define TOWER_COUNT                  10			//Total (alliance + horde)

#define TOWER_HEALTH_BASE            100000
#define NEXUS_HEALTH_BASE            100000

#define ITEM_ON_PVP_KILL             20558      //Recu par les joueur en quantité BASE_COUNT_ITEM_PVP_PLAYER ou BASE_COUNT_ITEM_PVP_CREATURE respectivement lorsqu'il tue un joueur ou une créature
#define BASE_COUNT_ITEM_PVP_PLAYER   30
#define BASE_COUNT_ITEM_PVP_CREATURE 3

enum BG_ICW_ObjectTypes
{
	OBJECT_DOME_A     = 0,  //Dome de l'alliance
	OBJECT_DOME_H     = 1,  //Dome de la horde

	BG_ICW_OBJECT_MAX = 2
};

enum BG_ICW_ObjectEntry
{
	OBJECT_DOME_ENTRY = 184720 //Même dome pour l'alliance et pour la horde
};

enum DOORS_ICW
{
	DOOR_DOME = 184720
};

enum BG_ICW_WorldStates
{
	BG_ICW_TIME_REMAINING = 4248
};

enum BG_ICW_Graveyards
{
	ICW_GRAVEYARD_MAIN_ALLIANCE = 1800,
	ICW_GRAVEYARD_MAIN_HORDE    = 1801
};

enum BG_ICW_CreatureTypes
{
	ICW_SPIRIT_MAIN_ALLIANCE = 0,
	ICW_SPIRIT_MAIN_HORDE    = 1,
	ICW_MINION_CAC_A	     = 2,
	ICW_MINION_CAST_A		 = 3,
	ICW_MINION_CAC_H		 = 4,
	ICW_MINION_CAST_H	     = 5,

	ICW_TOUR_H				 = 6,
	ICW_TOUR_A				 = 7,

	ICW_NEXUS_A				 = 8,
	ICW_NEXUS_H				 = 9,

	BG_CREATURE_MAX_ICW		 = 10
};

enum BG_ICW_CreatureEntry
{
	ICW_MINION_CAC_A_ENTRY  = 2000000,   //CAC alliance
	ICW_MINION_CAST_A_ENTRY = 200020,    //Caster alliance
	ICW_MINION_CAC_H_ENTRY  = 2000001,   //Cac horde
	ICW_MINION_CAST_H_ENTRY = 2000002,   //Caster horde

	ICW_TOUR_H_ENTRY		= 200011,    //Tours de la horde
	ICW_TOUR_A_ENTRY		= 200010,    //Tours de l'alliance

	ICW_NEXUS_A_ENTRY		= 200012,	 //Nexus de l'alliance
	ICW_NEXUS_H_ENTRY		= 200013     //Nexus de la horde
};

enum BG_ICW_WorldSates
{
	BG_ICW_STATE_TIMER_ACTIVE = 0,
	BG_ICW_STATE_TIMER		  = 1,
};

enum BG_ICW_TimerStates
{
	BG_ICW_STATE_MINION_TIMER_ACTIVE = 0,
	BG_ICW_STATE_MINION_TIMER_ASLEEP = 1
};


enum BG_ICW_SAY //id des textes dans trinity strings
{
	SAY_BG_STARTING_EVENT_GO     = 100000,
	SAY_BG_STARTING_EVENT_SECOND = 100001,
	SAY_BG_STARTING_EVENT_THIRD  = 100002,
	SAY_BG_STARTING_EVENT_FOURTH = 100003,

	SAY_BG_TOUR_A_KILLED	     = 100004,
	SAY_BG_TOUR_H_KILLED		 = 100005,

	SAY_BG_MINIONS				 = 100006
};

enum BG_ICW_SOUND //Sons repris = sons du goulet
{
	ICW_SOUND_TOWER_H_KILLED = 8173,
	ICW_SOUND_TOWER_A_KILLED = 8213,
	ICW_SOUND_MINION_POP     = 8192
};

class BattlegroundICW : public Battleground
{

public:

	//----------------------------------------
	//Constructeur et Destructeur
	//----------------------------------------
	BattlegroundICW();
	~BattlegroundICW();

	/* inherited from BattlegroundClass */
	void AddPlayer(Player* plr);
	void StartingEventCloseDoors();
	void StartingEventOpenDoors();
	bool SetupBattleground();
	void Reset();
	void FillInitialWorldStates(WorldPacket& data);
	virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);
	uint32							  GetRemainingTimeInMinutes() { return m_EndTimer ? (m_EndTimer - 1) / (MINUTE * IN_MILLISECONDS) + 1 : 0; }
	void PostUpdateImpl(uint32 diff);
	void HandleKillUnit(Creature* unit, Player* killer);
	void HandleKillPlayer(Player* player, Player* killer);
	void EndBattleground(uint32 winner);
	void RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket);

	//Fonctions membres ajoutées
	void ThrowNewMinionWave();


private:

	//Tours:
	//Droite = droite du point de vue du départ de l'alliance
	static const Position TourPositions[];
	static const Position NexusPositions[];

	uint32 m_HonorEndKills;
	uint32 m_EndTimer;
	uint32 m_TimeElapsed; //Depuis le debut du bg
	uint32 m_MinionTimerActiveState;
	uint32 m_MinionTimerState;
	std::vector<Creature*> m_AllianceTowers;
	std::vector<Creature*> m_HordeTowers;
	Creature* m_AllianceNexus;
	Creature* m_HordeNexus;

	uint32 m_TowerHPA;
	uint32 m_MinionHPA;
	uint32 m_NexusHPA;

	uint32 m_TowerHPH;
	uint32 m_MinionHPH;
	uint32 m_NexusHPH;
};
#endif