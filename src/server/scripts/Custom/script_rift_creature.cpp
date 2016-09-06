#include "ScriptMgr.h"
#include "RiftHandler.h"
#include "Unit.h"

enum Events
{
	EVENT_SUMMON_UNIT_BASE = 1,
	EVENT_SUMMON_UNIT_SECOND = 2,
	EVENT_SUMMON_UNIT_FINAL = 3,

	EVENT_SUMMON_PORTAL = 4,

	EVENT_GOTO_PHASE2 = 5,
	EVENT_GOTO_PHASE3 = 6,
	EVENT_GOTO_PHASE1 = 7,
	EVENT_GOTO_PHASE4 = 8
};

enum Phases
{
	PHASE_ALL = 0,
	PHASE_ONE = 1,
	PHASE_TWO = 2,
	PHASE_THREE = 3
};

#define BASE_UNIT_ENTRY 200062
#define SECOND_UNIT_ENTRY 200063
#define FINAL_UNIT_ENTRY 200064
#define PORTAL_ENTRY 177397

class script_rift_creature : public CreatureScript
{

public:

	script_rift_creature() : CreatureScript("script_rift_creature"){}

public:


	struct riftCreatureAI : public BossAI
	{
		GameObject* mPortal = NULL;
		Creature* mFinal = NULL;
		uint32 timeElapsed = 0;
		uint32 phase = 0;
		bool lastSummoned = false;
		uint32 subTimeElapsed = 0;

		uint32 summonCount = 0;
		uint32 encounters = 0;

		riftCreatureAI(Creature* creature) : BossAI(creature, 1500001)
		{
		}

		void JustDied(Unit* killer)
		{
			Player* p = killer->ToPlayer();
			if (!p)
			{
				sRH->OnRiftClosed(me->GetGUID());
				return;
			}

			Group* group = p->GetGroup();
			if (group)
			{
				for (auto it = group->GetMemberSlots().begin(); it != group->GetMemberSlots().end(); it++)
				{
					uint32 guid = it->guid;
					Player* cp = sObjectAccessor->FindPlayer(guid);
					if (!cp)
						continue;
					cp->SetHonorPoints(p->GetHonorPoints() + 20000);
					cp->SetArenaPoints(p->GetArenaPoints() + 50);
					cp->SetReputation(2002, cp->GetReputation(2002) + 1000);
				}
			}
			else {
				p->SetHonorPoints(p->GetHonorPoints() + 20000);
				p->SetArenaPoints(p->GetArenaPoints() + 50);
				p->SetReputation(2002, p->GetReputation(2002) + 1000);
			}
			sRH->OnRiftClosed(me->GetGUID());
		}

		void Reset() override
		{
			timeElapsed = 0;
			phase = 0;
			lastSummoned = false;
			subTimeElapsed = 0;
			encounters = 0;
			summonCount = 0;
			if (mPortal)
				mPortal->RemoveFromWorld();
		}

		void UpdateAI(uint32 diff)
		{
			if (me->IsInCombat())
			{
				sRH->SetUnderAttack(true);
				if (me->getAttackers().size() != encounters)
				{
					int diff = me->getAttackers().size() - encounters;
					me->SetMaxHealth(me->GetMaxHealth() + diff);
					me->SetHealth((me->GetHealth() + diff) * 500000);
				}

			}
			else sRH->SetUnderAttack(false);

			if (!UpdateVictim())
			{
				if (mPortal)
					mPortal->RemoveFromWorld();
				return;
			}

			events.Update(diff);

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SUMMON_PORTAL:
					if (!mPortal)
						me->SummonGameObject(PORTAL_ENTRY, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.f, me->GetOrientation(), 0.f, 0.f, 0.f, 0.f);
					break;

				case EVENT_SUMMON_UNIT_BASE:
				{
											   summonCount++;
											   if (summonCount >= 10)
											   {
												   events.SetPhase(PHASE_TWO);
												   summonCount = 0;
												   break;
											   }
											   Creature* creature = me->SummonCreature(BASE_UNIT_ENTRY, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
											   creature->setFaction(1885);
											   creature->JumpTo(2.f, 2.f, true);
											   creature->Attack(sObjectAccessor->GetUnit(*me, me->GetTarget()), true);


				}
					break;

				case EVENT_SUMMON_UNIT_SECOND:
				{
												 summonCount++;
												 if (summonCount >= 10)
												 {
													 events.SetPhase(PHASE_THREE);
													 summonCount = 0;
													 break;
												 }
												 Creature* creature = me->SummonCreature(SECOND_UNIT_ENTRY, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
												 creature->setFaction(1885);
												 creature->JumpTo(2.f, 2.f, true);
												 creature->Attack(sObjectAccessor->GetUnit(*me, me->GetTarget()), true);

				}
					break;

				case EVENT_SUMMON_UNIT_FINAL:
				{
												Creature* creature = me->SummonCreature(FINAL_UNIT_ENTRY, me->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
												creature->setFaction(1885);
												creature->JumpTo(2.f, 2.f, true);
												creature->Attack(sObjectAccessor->GetUnit(*me, me->GetTarget()), true);

				}
					break;


				}
			}
		}


		void EnterCombat(Unit* /*who*/) override
		{
			_EnterCombat();

			encounters = me->getAttackers().size();

			sRH->SetUnderAttack(true);

			events.SetPhase(PHASE_ONE);

			events.ScheduleEvent(EVENT_SUMMON_PORTAL, 1000, 0, PHASE_ONE);

			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 1500, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 2000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 4000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 6000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 8000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 10000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 12000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 14000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 16000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 18000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 20000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 22000, 0, PHASE_ONE);


			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 20000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 22000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 24000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 26000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 28000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 30000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 32000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 34000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 36000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 38000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 40000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 42000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 42000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 42000, 0, PHASE_ONE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_BASE, 42000, 0, PHASE_ONE);


			events.ScheduleEvent(EVENT_SUMMON_UNIT_FINAL, 43000, 0, PHASE_THREE);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);
			events.ScheduleEvent(EVENT_SUMMON_UNIT_SECOND, 80000, 0, PHASE_TWO);


		}


		void EnterEvadeMode() override
		{
			BossAI::_EnterEvadeMode();
			if (mPortal)
			{
				mPortal->RemoveFromWorld();
			}
		}

	};


	void OnUpdate(Creature* creature, uint32 /*diff*/)
	{
		if (!creature->HasAura(RIFT_AURA))
			creature->AddAura(RIFT_AURA, creature);


		/*Map::PlayerList const& plList = creature->GetMap()->GetPlayers();

		for (auto it = plList.begin(); it != plList.end(); ++it)
		{
		Player* player = it->GetSource();

		if (player->GetAreaId() == creature->GetAreaId())
		sRH->OnPlayerGettingClose();
		if (player->GetAreaId() != creature->GetAreaId())
		{

		}
		} */
	}


	CreatureAI* GetAI(Creature* creature) const override
	{
		return new riftCreatureAI(creature);
	}

};

void AddSC_script_rift_creature()
{
	new script_rift_creature();
}