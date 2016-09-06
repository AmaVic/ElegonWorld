#include "Object.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundICW.h"
#include "Creature.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "WorldPacket.h"
#include "Language.h"
#include "MapManager.h"
#include "WorldSession.h"
#include "Creature.h"
#include "SmartAI.h"
#include "BattlegroundScore.h"

//Constantes:
const Position BattlegroundICW::TourPositions[] = 
{ 
	Position(-983.151428f, -1026.978149f, -29.550127f, 6.261430f), //TOUR GAUCHE ALLIANCE
	Position(-883.708801f, -1122.405396f, -29.550127f, 0.669400f), //TOUR CENTRE ALLIANCE
	Position(-802.974182f, -1218.309326f, -29.550043f, 0.292177f), //TOUR DROITE ALLIANCE
	Position(-857.688293f, -977.366760f, -29.550022f, -3.640590f), //TOUR GAUCHE HORDE
	Position(-819.787354f, -1073.421875f, -29.549856f, 3.702931f), //TOUR CENTRE HORDE
	Position(-713.977234f, -1119.159790f, -29.550179f, 4.582577f), //TOUR DROITE HORDE
	Position(-937.875549f, -1164.624512f, -29.550270f, 1.405457f), //TOUR 1 NEXUS A
	Position(-929.361206f, -1174.067749f, -29.558826f, 6.082502f), //TOUR 2 NEXUS A
	Position(-752.823975f, -1031.154907f, -29.241770f, 5.143950f), //TOUR 1 NEXUS H
	Position(-764.398621f, -1015.248352f, -29.434471f, 2.768121f)  //TOUR 2 NEXUS H
};

const Position BattlegroundICW::NexusPositions[] =
{
	Position(-937.350830f, -1171.982300f, -29.583185f, 0.739439f), //NEXUS ALLIANCE
	Position(-755.033386f, -1022.541748f, -29.163715f, 3.849616f), //NEXUS HORDE
};

BattlegroundICW::BattlegroundICW() : m_AllianceTowers(), m_HordeTowers()
{
	BgObjects.resize(BG_ICW_OBJECT_MAX);
	BgCreatures.resize(BG_CREATURE_MAX_ICW);

	//Définition des messages affichés
	StartMessageIds[BG_STARTING_EVENT_FIRST] = SAY_BG_STARTING_EVENT_SECOND;
	StartMessageIds[BG_STARTING_EVENT_SECOND] = SAY_BG_STARTING_EVENT_THIRD;
	StartMessageIds[BG_STARTING_EVENT_THIRD] = SAY_BG_STARTING_EVENT_FOURTH;
	StartMessageIds[BG_STARTING_EVENT_FOURTH] = SAY_BG_STARTING_EVENT_GO;

	//Gestion des minions: timers
	m_TimeElapsed = 0;
	m_MinionTimerState = BG_ICW_STATE_MINION_TIMER_ASLEEP;
	
}

BattlegroundICW::~BattlegroundICW()
{
}

void BattlegroundICW::AddPlayer(Player *plr)
{
	Battleground::AddPlayer(plr);

	//create score and add it to map, default values are set in constructor
	BattlegroundScore* sc = new BattlegroundScore(plr->GetGUID());

	PlayerScores[plr->GetGUID()] = sc;

	plr->AddItem(ITEM_ON_PVP_KILL, 30);
}

bool BattlegroundICW::SetupBattleground()
{
	AddObject(OBJECT_DOME_A, OBJECT_DOME_ENTRY, -984.757568f, -1216.756863f, -22.506008f, 0.6663260f, 0.f, 0.f, 0.f, 0.f, RESPAWN_IMMEDIATELY);
	AddObject(OBJECT_DOME_H, OBJECT_DOME_ENTRY, -719.856812f, -985.479767f, -22.481642f, 3.938380f, 0.f, 0.f, 0.f, 0.f, RESPAWN_IMMEDIATELY);

	return true;
}

void BattlegroundICW::Reset()
{
	//call parent's class reset
	Battleground::Reset();

	for (uint32 i = 0; i < BG_TEAMS_COUNT; ++i)
		m_TeamScores[i] = 0;

	m_HonorEndKills = 2;
	m_EndTimer = BG_ICW_TIME_LIMIT;
}

WorldSafeLocsEntry const* BattlegroundICW::GetClosestGraveYard(Player* player)
{
	if (player->GetTeam() == ALLIANCE)
		return sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_ALLIANCE);
	else
		return sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_HORDE);
}


void BattlegroundICW::FillInitialWorldStates(WorldPacket& data)
{
	data << uint32(BG_ICW_TIME_REMAINING) << (uint32)GetRemainingTimeInMinutes();
}

void BattlegroundICW::RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket)
{
	Battleground::RemovePlayerAtLeave(guid, Transport, SendPacket);
	Player* p = sObjectAccessor->FindPlayer(guid);
	if (!p)
		return;

	p->AddItem(20558, -1000000);
	p->RemoveAllAuras();
}



void BattlegroundICW::StartingEventCloseDoors()
{
	DoorClose(OBJECT_DOME_A);
	DoorClose(OBJECT_DOME_H);
	SpawnBGObject(OBJECT_DOME_A, RESPAWN_IMMEDIATELY);
	SpawnBGObject(OBJECT_DOME_H, RESPAWN_IMMEDIATELY);
}

void BattlegroundICW::StartingEventOpenDoors()
{
	DoorOpen(OBJECT_DOME_A);
	DoorOpen(OBJECT_DOME_H);
	SpawnBGObject(OBJECT_DOME_A, RESPAWN_ONE_DAY);
	SpawnBGObject(OBJECT_DOME_H, RESPAWN_ONE_DAY);

	m_TimeElapsed = 0;
	m_MinionTimerActiveState = BG_ICW_STATE_MINION_TIMER_ACTIVE;
	m_MinionTimerState = 0;


	uint64 totalPlayersHPAlliance = 0;
	uint64 totalPlayersHPHorde = 0;
	for (auto bgplayer : GetPlayers())
	{
		uint64 plGUID = bgplayer.first;
		Player* p = sObjectAccessor->FindPlayer(plGUID);
		if (!p)
			continue;

		if (p->GetTeamId() == TEAM_HORDE)
		{
			totalPlayersHPAlliance += p->GetMaxHealth();
		}
		else totalPlayersHPHorde += p->GetMaxHealth();

	}

	m_MinionHPA = totalPlayersHPAlliance / 5;
	m_TowerHPA = totalPlayersHPAlliance;
	m_NexusHPA = totalPlayersHPAlliance * 2;

	m_MinionHPH = totalPlayersHPHorde / 5;
	m_TowerHPH = totalPlayersHPHorde;
	m_NexusHPH = totalPlayersHPHorde * 2;

	//---------------------------------------------------------------------
	//Ajout des tours
	for (int i = 0; i < TOWER_COUNT; i++)
	{
		if (i <= 2 || (i >= 6 && i < 8)) //Tours Alliance
		{
			Creature* tower = AddCreature(ICW_TOUR_A_ENTRY, ICW_TOUR_A, TourPositions[i], TEAM_ALLIANCE, RESPAWN_ONE_DAY);
			tower->setFaction(FACTION_ALLIANCE);
			tower->SetMaxHealth(m_TowerHPA);
			tower->SetHealth(tower->GetMaxHealth());
			tower->SetMaxPower(POWER_MANA, 1000000);
			tower->SetPower(POWER_MANA, tower->GetMaxPower(POWER_MANA));
			m_AllianceTowers.push_back(tower);

		}
		else if ((i > 2 && i < 6) || i >= 8) //Tours Horde
		{
			Creature* tower = AddCreature(ICW_TOUR_H_ENTRY, ICW_TOUR_H, TourPositions[i], TEAM_HORDE, RESPAWN_ONE_DAY);
			tower->setFaction(FACTION_HORDE);
			tower->SetMaxHealth(m_TowerHPH);
			tower->SetHealth(tower->GetMaxHealth());
			tower->SetMaxPower(POWER_MANA, 1000000);
			tower->SetPower(POWER_MANA, tower->GetMaxPower(POWER_MANA));
			m_HordeTowers.push_back(tower);
		}
	}

	//Ajout des nexus
	m_AllianceNexus = AddCreature(ICW_NEXUS_A_ENTRY, ICW_NEXUS_A, NexusPositions[0], TEAM_ALLIANCE, RESPAWN_ONE_DAY);
	m_AllianceNexus->setFaction(FACTION_ALLIANCE);
	m_AllianceNexus->SetMaxHealth(m_NexusHPA);
	m_AllianceNexus->SetHealth(m_AllianceNexus->GetMaxHealth());
	m_AllianceNexus->SetReactState(REACT_PASSIVE);


	m_HordeNexus = AddCreature(ICW_NEXUS_H_ENTRY, ICW_NEXUS_H, NexusPositions[1], TEAM_HORDE, RESPAWN_ONE_DAY);
	m_HordeNexus->setFaction(FACTION_HORDE);
	m_HordeNexus->SetMaxHealth(m_NexusHPH);
	m_HordeNexus->SetHealth(m_HordeNexus->GetMaxHealth());
	m_HordeNexus->SetReactState(REACT_PASSIVE);

	WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_ALLIANCE);
	AddSpiritGuide(ICW_SPIRIT_MAIN_ALLIANCE, sg->x, sg->y, sg->z, 0.f, TEAM_ALLIANCE);

	sg = sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_HORDE);
	AddSpiritGuide(ICW_SPIRIT_MAIN_HORDE, sg->x, sg->y, sg->z, 0.f, TEAM_HORDE);

	//---------------------------------------------------------------------

}


void BattlegroundICW::ThrowNewMinionWave()
{
	SendMessageToAll(SAY_BG_MINIONS, CHAT_MSG_BATTLEGROUND);

	//CACS: Alliance: droite
	Creature* c = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_1_X, MINION_POS_A_1_Y, MINION_POS_A_1_Z, MINION_POS_A_1_O, TEAM_ALLIANCE, 1 *MINUTE * IN_MILLISECONDS);
	Creature* c2 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_1_X - 2.f, MINION_POS_A_1_Y, MINION_POS_A_1_Z, MINION_POS_A_1_O, TEAM_ALLIANCE, 1 *MINUTE * IN_MILLISECONDS);
	Creature* c3 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_1_X - 4.f, MINION_POS_A_1_Y, MINION_POS_A_1_Z, MINION_POS_A_1_O, TEAM_ALLIANCE, 1 *MINUTE * IN_MILLISECONDS);
	
	//Casters: Alliance: droite
	Creature* c4 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_1_X - 6.f, MINION_POS_A_1_Y, MINION_POS_A_1_Z, MINION_POS_A_1_O, TEAM_ALLIANCE, 1 *MINUTE * IN_MILLISECONDS);
	Creature* c5 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_1_X - 8.f, MINION_POS_A_1_Y, MINION_POS_A_1_Z, MINION_POS_A_1_O, TEAM_ALLIANCE, 1 *MINUTE * IN_MILLISECONDS);


	//CACS: Alliance: gauche
	Creature* c6 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_2_X, MINION_POS_A_2_Y, MINION_POS_A_2_Z, MINION_POS_A_2_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c7 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_2_X, MINION_POS_A_2_Y-2.f, MINION_POS_A_2_Z, MINION_POS_A_2_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c8 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_2_X, MINION_POS_A_2_Y-4.f, MINION_POS_A_2_Z, MINION_POS_A_2_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);

	//Casters:  Alliance: gauche
	Creature* c9 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_2_X, MINION_POS_A_2_Y-6.f, MINION_POS_A_2_Z, MINION_POS_A_2_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c10 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_2_X, MINION_POS_A_2_Y-8.f, MINION_POS_A_2_Z, MINION_POS_A_2_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);

	//CACS: Alliance:  centre
	Creature* c11 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_3_X, MINION_POS_A_3_Y, MINION_POS_A_3_Z, MINION_POS_A_3_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c12 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_3_X - 2.f, MINION_POS_A_3_Y-2.f, MINION_POS_A_3_Z, MINION_POS_A_3_O, TEAM_ALLIANCE,   1 *MINUTE * IN_MILLISECONDS);
	Creature* c13 = AddCreature(ICW_MINION_CAC_A_ENTRY, ICW_MINION_CAC_A, MINION_POS_A_3_X - 4.f, MINION_POS_A_3_Y-4.f, MINION_POS_A_3_Z, MINION_POS_A_3_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);

	//Cast: Alliance: Centre
	Creature* c14 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_3_X - 6.f, MINION_POS_A_3_Y-6.f, MINION_POS_A_3_Z, MINION_POS_A_3_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c15 = AddCreature(ICW_MINION_CAST_A_ENTRY, ICW_MINION_CAST_A, MINION_POS_A_3_X - 8.f, MINION_POS_A_3_Y-8.f, MINION_POS_A_3_Z, MINION_POS_A_3_O, TEAM_ALLIANCE,  1 *MINUTE * IN_MILLISECONDS);
	
	
	//CACS: Horde: droite
	Creature* c16 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_1_X, MINION_POS_H_1_Y, MINION_POS_H_1_Z, MINION_POS_H_1_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c17 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_1_X-2.f, MINION_POS_H_1_Y, MINION_POS_H_1_Z, MINION_POS_H_1_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c18 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_1_X-4.f, MINION_POS_H_1_Y, MINION_POS_H_1_Z, MINION_POS_H_1_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	
	//Cast: Horde: droite
	Creature* c19 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_1_X-6.f, MINION_POS_H_1_Y, MINION_POS_H_1_Z, MINION_POS_H_1_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c20 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_1_X-8.f, MINION_POS_H_1_Y, MINION_POS_H_1_Z, MINION_POS_H_1_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);

	//CAC: Horde: gauche
	Creature* c21 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_2_X, MINION_POS_H_2_Y, MINION_POS_H_2_Z, MINION_POS_H_2_O, TEAM_HORDE, 1 *MINUTE * IN_MILLISECONDS);
	Creature* c22 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_2_X, MINION_POS_H_2_Y-2.f, MINION_POS_H_2_Z, MINION_POS_H_2_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c23 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_2_X, MINION_POS_H_2_Y-4.f, MINION_POS_H_2_Z, MINION_POS_H_2_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	//CAST: Horde: gauche
	Creature* c24 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_2_X, MINION_POS_H_2_Y - 6.f, MINION_POS_H_2_Z, MINION_POS_H_2_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c25 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_2_X, MINION_POS_H_2_Y - 8.f, MINION_POS_H_2_Z, MINION_POS_H_2_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);

	//CAC: Horde: centre:
	Creature* c26 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_3_X, MINION_POS_H_3_Y, MINION_POS_H_3_Z, MINION_POS_H_3_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c27 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_3_X-2.f, MINION_POS_H_3_Y-2.f, MINION_POS_H_3_Z, MINION_POS_H_3_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c28 = AddCreature(ICW_MINION_CAC_H_ENTRY, ICW_MINION_CAC_H, MINION_POS_H_3_X-4.f, MINION_POS_H_3_Y-4.f, MINION_POS_H_3_Z, MINION_POS_H_3_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);

	//CAST: Horde: centre
	Creature* c29 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_3_X - 6.f, MINION_POS_H_3_Y - 6.f, MINION_POS_H_3_Z, MINION_POS_H_3_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);
	Creature* c30 = AddCreature(ICW_MINION_CAST_H_ENTRY, ICW_MINION_CAST_H, MINION_POS_H_3_X - 8.f, MINION_POS_H_3_Y - 8.f, MINION_POS_H_3_Z, MINION_POS_H_3_O, TEAM_HORDE,  1 *MINUTE * IN_MILLISECONDS);


	//Paths and speed

	
	c->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c2->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c3->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c4->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c5->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c6->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c7->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c8->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c9->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c10->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c11->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c12->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c13->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c14->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c15->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c16->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c17->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c18->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c19->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c20->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c21->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c22->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c23->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c24->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c25->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c26->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c27->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c28->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c29->SetSpeed(MOVE_WALK, UNITS_SPEED);
	c30->SetSpeed(MOVE_WALK, UNITS_SPEED);




	//Alliance;droite
	c->GetMotionMaster()->MovePath(1000, false);
	c2->GetMotionMaster()->MovePath(1000, false);
	c3->GetMotionMaster()->MovePath(1000, false);
	c4->GetMotionMaster()->MovePath(1000, false);
	c5->GetMotionMaster()->MovePath(1000, false);
	//Alliance;gauche
	c6->GetMotionMaster()->MovePath(1001, false);
	c7->GetMotionMaster()->MovePath(1001, false);
	c8->GetMotionMaster()->MovePath(1001, false);
	c9->GetMotionMaster()->MovePath(1001, false);
	c10->GetMotionMaster()->MovePath(1001, false);
	//Alliance: centre
	c11->GetMotionMaster()->MovePath(1006, false);
	c12->GetMotionMaster()->MovePath(1006, false);
	c13->GetMotionMaster()->MovePath(1006, false);
	c14->GetMotionMaster()->MovePath(1006, false);
	c15->GetMotionMaster()->MovePath(1006, false);
	//Horde:droite
	c16->GetMotionMaster()->MovePath(1003, false);
	c17->GetMotionMaster()->MovePath(1003, false);
	c18->GetMotionMaster()->MovePath(1003, false);
	c19->GetMotionMaster()->MovePath(1003, false);
	c20->GetMotionMaster()->MovePath(1003, false);
	//horde: gauche:
	c21->GetMotionMaster()->MovePath(1004, false);
	c22->GetMotionMaster()->MovePath(1004, false);
	c23->GetMotionMaster()->MovePath(1004, false);
	c24->GetMotionMaster()->MovePath(1004, false);
	c25->GetMotionMaster()->MovePath(1004, false);
	//Horde: centre:
	c26->GetMotionMaster()->MovePath(1007, false);
	c27->GetMotionMaster()->MovePath(1007, false);
	c28->GetMotionMaster()->MovePath(1007, false);
	c29->GetMotionMaster()->MovePath(1007, false);
	c30->GetMotionMaster()->MovePath(1007, false);

	
	c->setFaction(FACTION_ALLIANCE);
	c2->setFaction(FACTION_ALLIANCE);
	c3->setFaction(FACTION_ALLIANCE);
	c4->setFaction(FACTION_ALLIANCE);
	c5->setFaction(FACTION_ALLIANCE);
	c6->setFaction(FACTION_ALLIANCE);
	c7->setFaction(FACTION_ALLIANCE);
	c8->setFaction(FACTION_ALLIANCE);
	c9->setFaction(FACTION_ALLIANCE);
	c10->setFaction(FACTION_ALLIANCE);
	c11->setFaction(FACTION_ALLIANCE);
	c12->setFaction(FACTION_ALLIANCE);
	c13->setFaction(FACTION_ALLIANCE);
	c14->setFaction(FACTION_ALLIANCE);
	c15->setFaction(FACTION_ALLIANCE);

	c16->setFaction(FACTION_HORDE);
	c17->setFaction(FACTION_HORDE);
	c18->setFaction(FACTION_HORDE);
	c19->setFaction(FACTION_HORDE);
	c20->setFaction(FACTION_HORDE);
	c21->setFaction(FACTION_HORDE);
	c22->setFaction(FACTION_HORDE);
	c23->setFaction(FACTION_HORDE);
	c24->setFaction(FACTION_HORDE);
	c25->setFaction(FACTION_HORDE);
	c26->setFaction(FACTION_HORDE);
	c27->setFaction(FACTION_HORDE);
	c28->setFaction(FACTION_HORDE);
	c29->setFaction(FACTION_HORDE);
	c30->setFaction(FACTION_HORDE);


	//c -> c15: alliance
	c->SetMaxHealth(m_MinionHPA);
	c->SetHealth(c->GetMaxHealth());

	c2->SetMaxHealth(m_MinionHPA);
	c2->SetHealth(c2->GetMaxHealth());

	c3->SetMaxHealth(m_MinionHPA);
	c3->SetHealth(c3->GetMaxHealth());

	c4->SetMaxHealth(m_MinionHPA);
	c4->SetHealth(c4->GetMaxHealth());

	c5->SetMaxHealth(m_MinionHPA);
	c5->SetHealth(c->GetMaxHealth());

	c6->SetMaxHealth(m_MinionHPA);
	c6->SetHealth(c->GetMaxHealth());

	c7->SetMaxHealth(m_MinionHPA);
	c7->SetHealth(c->GetMaxHealth());

	c8->SetMaxHealth(m_MinionHPA);
	c8->SetHealth(c->GetMaxHealth());

	c9->SetMaxHealth(m_MinionHPA);
	c9->SetHealth(c->GetMaxHealth());

	c10->SetMaxHealth(m_MinionHPA);
	c10->SetHealth(c->GetMaxHealth());

	c11->SetMaxHealth(m_MinionHPA);
	c11->SetHealth(c->GetMaxHealth());


	c12->SetMaxHealth(m_MinionHPA);
	c12->SetHealth(c->GetMaxHealth());

	c13->SetMaxHealth(m_MinionHPA);
	c13->SetHealth(c->GetMaxHealth());

	c14->SetMaxHealth(m_MinionHPA);
	c14->SetHealth(c->GetMaxHealth());

	c15->SetMaxHealth(m_MinionHPA);
	c15->SetHealth(c->GetMaxHealth());

	c16->SetMaxHealth(m_MinionHPH);
	c16->SetHealth(c16->GetMaxHealth());

	c17->SetMaxHealth(m_MinionHPH);
	c17->SetHealth(c16->GetMaxHealth());

	c18->SetMaxHealth(m_MinionHPH);
	c18->SetHealth(c16->GetMaxHealth());

	c19->SetMaxHealth(m_MinionHPH);
	c19->SetHealth(c16->GetMaxHealth());

	c20->SetMaxHealth(m_MinionHPH);
	c20->SetHealth(c16->GetMaxHealth());


	c21->SetMaxHealth(m_MinionHPH);
	c21->SetHealth(c16->GetMaxHealth());

	c22->SetMaxHealth(m_MinionHPH);
	c22->SetHealth(c16->GetMaxHealth());

	c23->SetMaxHealth(m_MinionHPH);
	c23->SetHealth(c16->GetMaxHealth());

	c24->SetMaxHealth(m_MinionHPH);
	c24->SetHealth(c16->GetMaxHealth());

	c25->SetMaxHealth(m_MinionHPH);
	c25->SetHealth(c16->GetMaxHealth());

	c26->SetMaxHealth(m_MinionHPH);
	c26->SetHealth(c16->GetMaxHealth());

	c27->SetMaxHealth(m_MinionHPH);
	c27->SetHealth(c16->GetMaxHealth());

	c28->SetMaxHealth(m_MinionHPH);
	c28->SetHealth(c16->GetMaxHealth());

	c29->SetMaxHealth(m_MinionHPH);
	c29->SetHealth(c16->GetMaxHealth());

	c30->SetMaxHealth(m_MinionHPH);
	c30->SetHealth(c16->GetMaxHealth());
}

void BattlegroundICW::PostUpdateImpl(uint32 diff)
{
	if (GetStatus() == STATUS_IN_PROGRESS)
	{

		m_TimeElapsed += diff;

		if (m_MinionTimerActiveState == BG_ICW_STATE_MINION_TIMER_ACTIVE)
		{
			m_MinionTimerState += diff;

			if (m_MinionTimerState >= TIME_BETWEEN_MINION_WAVES)
			{
				//Reset du timer
				m_MinionTimerState = 0;
				//Apparition des minions
				ThrowNewMinionWave();
			}
		}

		//On regarde s'il y a des ennemis dans un cimetière
		if (m_TimeElapsed >= 10000)
		{
			for (auto it = PlayerScores.begin(); it != PlayerScores.end(); ++it)
			{
				Player* p = sObjectAccessor->FindPlayer(it->first);
				if (!p)
					continue;

				float dist = 0;
				if (p->GetTeamId() == TEAM_HORDE)
				{
					//On récup la distance par rapport au cimetière alliance
					WorldSafeLocsEntry const* loc = sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_ALLIANCE);
					dist = std::sqrtf(std::pow(loc->x - p->GetPositionX(), 2) + std::pow(loc->y - p->GetPositionY(), 2) + std::pow(loc->z - p->GetPositionZ(), 2));
				}
				else{
					//On récup la distance par rapport au cimetière horde
					WorldSafeLocsEntry const* loc = sWorldSafeLocsStore.LookupEntry(ICW_GRAVEYARD_MAIN_HORDE);
					dist = std::sqrtf(std::pow(loc->x - p->GetPositionX(), 2) + std::pow(loc->y - p->GetPositionY(), 2) + std::pow(loc->z - p->GetPositionZ(), 2));
				}

				//Si le joueur est trop près, on le TP à son Spawn et on lui dit
				if (dist <= 35.f)
				{
					WorldSafeLocsEntry const* loc = GetClosestGraveYard(p);
					p->TeleportTo(loc->map_id, loc->x, loc->y, loc->z, 0.f);
					p->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas aller dans le cimetiere ennemi !");
				}
			}

			m_TimeElapsed = 0;

		}

		if (m_AllianceNexus->isDead())
		{
			EndBattleground(TEAM_HORDE);
		}
		
		if (m_HordeNexus->isDead())
		{
			EndBattleground(TEAM_ALLIANCE);
		}

	}

}

void BattlegroundICW::HandleKillUnit(Creature* unit, Player* killer)
{
	killer->AddItem(ITEM_ON_PVP_KILL, BASE_COUNT_ITEM_PVP_CREATURE);

	uint32 entry = unit->GetEntry();

	std::function<uint32(const std::vector<Creature*>&)> left = [this](const std::vector<Creature*>& crea)
	{
		uint32 count = 0;
		for (auto tower : crea)
		{
			if (tower->IsAlive())
				count++;
		}

		return count;
	};

	if (entry == ICW_TOUR_A_ENTRY)
	{
		
		SendWarningToAll(SAY_BG_TOUR_A_KILLED, left(m_AllianceTowers));
		PlaySoundToAll(ICW_SOUND_TOWER_A_KILLED);
		if (left(m_AllianceTowers) == 0)
		{
			m_AllianceNexus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		}
	}

	if (entry == ICW_TOUR_H_ENTRY)
	{
		SendWarningToAll(SAY_BG_TOUR_H_KILLED, left(m_HordeTowers));
		PlaySoundToAll(ICW_SOUND_TOWER_H_KILLED);
		if (left(m_HordeTowers) == 0)
		{
			m_HordeNexus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		}
	}

	//Alliance win
	if (entry == ICW_NEXUS_A_ENTRY)
	{
		EndBattleground(TEAM_HORDE);
	}
	//Horde win
	if (entry == ICW_NEXUS_H_ENTRY)
	{
		EndBattleground(TEAM_ALLIANCE);
	}
}



void BattlegroundICW::HandleKillPlayer(Player* player, Player* killer)
{
	if (GetStatus() != STATUS_IN_PROGRESS)
		return;
	
	BattlegroundScoreMap::iterator itr = PlayerScores.find(player->GetGUID());
	int killCount = ((BattlegroundScore*)itr->second)->HonorableKills;
	int deathCount = ((BattlegroundScore*)itr->second)->Deaths;
	int diff = killCount - deathCount;
	int total = BASE_COUNT_ITEM_PVP_PLAYER + (diff >= 0 ? diff : 0);
	killer->AddItem(ITEM_ON_PVP_KILL, total);

	Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundICW::EndBattleground(uint32 winner)
{
	for (auto itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
	{
		int killCount = ((BattlegroundScore*)itr->second)->HonorableKills;
		Player* p = sObjectAccessor->FindPlayer(itr->first);
		if (!p)
			continue;

		p->SetHonorPoints(p->GetHonorPoints() + killCount * 1000);
		p->RemoveAllAuras();

		if (p->GetTeamId() == winner)
		{
			p->SetReputation(72, (p->GetReputation(72) + 2000));
		} else p->SetReputation(76, (p->GetReputation(76) + 1000));

		QueryResult checkResult = CharacterDatabase.PQuery("SELECT * FROM loe_ranking WHERE playerGUID=%u", p->GetGUID());
		if (!checkResult)
		{
			//On ajoute la ligne
			if (p->GetTeamId() == winner)
			{
				CharacterDatabase.PExecute("INSERT INTO loe_ranking VALUES(%u, 1, 0)", p->GetGUID());
			}
			else CharacterDatabase.PExecute("INSERT INTO loe_ranking VALUES(%u, 0, 1)", p->GetGUID());
		}
		else {
			//Mise à jour de la ligne:
			if (p->GetTeamId() == winner)
			{
				CharacterDatabase.PExecute("UPDATE loe_ranking SET victories=victories+1 WHERE playerGUID=%u", p->GetGUID());
			}
			else CharacterDatabase.PExecute("UPDATE loe_ranking SET defeats=defeats+1 WHERE playerGUID=%u", p->GetGUID());
		}
	}

	Battleground::EndBattleground(winner);
}