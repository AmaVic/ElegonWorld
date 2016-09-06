

#include "ScriptMgr.h"
#include "ZGCustom.h"

enum Spells
{
	SPELL_FRACASS = 25051,
	SPELL_CIRCULAR_ATTACK = 40504,
	SPELL_AURA_P2 = 64328,
	SPELL_TOURBILOL = 69652,
	SPELL_STUN_AND_WEAK = 66758,
	SPELL_BONES_TEMPEST = 69076,
	SPELL_ENRAGE = 47008,
	SPELL_AOE_AFTER_JUMP = 63757,

	//CREATURES
	SPELL_STUN = 23454,
	SPELL_SHADOW_BOLT = 69528,
	SPELL_HEAL = 1064
};

enum Phases
{
	PHASE_ALL = 0,
	PHASE_ONE = 1,
	PHASE_TWO = 2
};

enum Events
{

	//----------BOSS-HIMSELF--------------
	//Perma
	EVENT_CAST_FRACASS = 1,
	EVENT_CAST_CIRCULAR_ATTACK = 2,
	//P1
	EVENT_SUMMON_ADDS = 3,
	//P2
	EVENT_SUMMON_SWARMS = 4,
	EVENT_SELECT_PLAYER = 5,
	EVENT_JUMP_SPAWN_POSITION = 6,
	EVENT_JUMP_TO_PLAYER = 7,
	EVENT_BOSS_AOE = 8,
	EVENT_ADD_WEAKNESS_AURA = 9,
	EVENT_ENRAGE = 10

	//-------------------------------------
};


enum JumperEvents
{
	EVENT_ADD_JUMP_TO_PLAYER = 1,
	EVENT_ADD_CAST_STUN = 2
};

enum HealerEvents
{
	EVENT_HEAL_FRIENDLY_UNIT = 1
};

enum CreatureEntries
{
	ADD_HEALER_ENTRY  = 787,
	ADD_JUMPER_ENTRY  = 27370,
	ADD_AOE_ENTRY     = 203,

	SWARM_UNITS_ENTRY = 33824
};

enum Say
{
	SAY_AGGRO = 0,
	SAY_CALL_ADDS = 0,
	SAY_SUMMON_BLADES = 0,
	SAY_SELECT_PLAYER = 0,
	SAY_ENRAGE = 0
};


const Position ADDS_POSITIONS[] =
{
	Position(-12001.347656f, -1701.941040f, 32.284241f, 1.183601f), //Healer
	Position(-12013.774414f, -1681.601929f, 32.284706f, 0.150017f), //Jumper
	Position(-12007.182617f, -1690.732666f, 32.284321f, 0.581992f)  //AOE
};

const Position SWARM_POSITIONS[] =
{
	Position(-12001.347656f, -1701.941040f, 32.284241f, 1.183601f),
	Position(-12013.774414f, -1681.601929f, 32.284706f, 0.150017f),
	Position(-11969.171875f, -1707.792847f, 32.009933f, 2.511149f),
	Position(-11982.182617f, -1676.310303f, 32.288147f, 4.176192f)
};

class script_boss_zg1 : public CreatureScript
{

public:

	//Constructeur
	script_boss_zg1() : CreatureScript("script_boss_zg1"){}

public:

	//Override

	struct boss_ZG1AI : public BossAI
	{
		uint32 summonCount;
		Unit* targetForJump;

		//Constructeur
		boss_ZG1AI(Creature* creature) : BossAI(creature, DATA_ZG_1), summonCount(0), targetForJump(nullptr){}

		//Overriding
		void EnterCombat(Unit* /*who*/) override
		{
			//Message:
			Talk(SAY_AGGRO);

			_EnterCombat();

			//Vars reset
			summonCount = 0;
			targetForJump = nullptr;

			//Permanently
			events.ScheduleEvent(EVENT_CAST_FRACASS, urand(5000, 7000), 0, PHASE_ALL);
			events.ScheduleEvent(EVENT_CAST_CIRCULAR_ATTACK, urand(4000, 6000), 0, PHASE_ALL);
			//P1
			events.ScheduleEvent(EVENT_SUMMON_ADDS, 20000, 0, PHASE_ONE);

			//Enrage: 255000 = 4:15
			//150000 = 2:30
			events.ScheduleEvent(EVENT_ENRAGE, 255000, 0, PHASE_ALL); //Enrage après 4:15
		}

		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			uint32 timeElapsed = events.GetTimer();
			//Après 10s, on passe en P1
			if (timeElapsed >= 10000 && events.GetPhaseMask() < PHASE_ONE)
			{
				events.SetPhase(PHASE_ONE);
			}

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				//Perma
				case EVENT_CAST_FRACASS:
					if (!me->HasAura(SPELL_STUN_AND_WEAK))
						DoCastVictim(SPELL_FRACASS, true);
					events.ScheduleEvent(EVENT_CAST_FRACASS, urand(5000, 7000), 0, PHASE_ALL);
					break;

				case EVENT_CAST_CIRCULAR_ATTACK:
					if (!me->HasAura(SPELL_STUN_AND_WEAK))
						DoCastVictim(SPELL_CIRCULAR_ATTACK, true);
					events.ScheduleEvent(EVENT_CAST_CIRCULAR_ATTACK, urand(4000, 6000), 0, PHASE_ALL);
					break;

				//P1
				case EVENT_SUMMON_ADDS:
					Talk(SAY_CALL_ADDS);
					DoSummon(ADD_HEALER_ENTRY, ADDS_POSITIONS[0]);
					DoSummon(ADD_JUMPER_ENTRY, ADDS_POSITIONS[1]);
					DoSummon(ADD_AOE_ENTRY,    ADDS_POSITIONS[2]);
					summonCount++;

					//Après 3 spawns, P2
					if (summonCount < 3)
					{
						events.ScheduleEvent(EVENT_SUMMON_ADDS, 20000, 0, PHASE_ONE);
					}
					else{
						events.SetPhase(PHASE_TWO);
						events.ScheduleEvent(EVENT_SUMMON_SWARMS, 3000, 0, PHASE_TWO);
						summonCount = 0;
					}
					break;
				//P2
				case EVENT_SUMMON_SWARMS:
					Talk(SAY_SUMMON_BLADES);
					//Pour chaque coin de Spawn
					for (int i = 0; i < 4; i++)
					{
						//On fait pop 4 adds
						for (int j = 0; j < 4; j++)
						{
							DoSummon(SWARM_UNITS_ENTRY, SWARM_POSITIONS[i])->SetCharmedBy(me, CharmType::CHARM_TYPE_CONVERT, 0);
						}

					}

					events.ScheduleEvent(EVENT_SELECT_PLAYER, 1000, 0, PHASE_TWO);

					break;

				case EVENT_SELECT_PLAYER:
				{
					Talk(SAY_SELECT_PLAYER);
					Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0.f, 200.f, true);
					target->AddAura(SPELL_AURA_P2, target);
					targetForJump = target;
					events.ScheduleEvent(EVENT_JUMP_SPAWN_POSITION, 1000, 0, PHASE_TWO);
				}
					break;

				case EVENT_JUMP_SPAWN_POSITION:
				{

					float x, y, z, o;
					me->GetHomePosition(x, y, z, o);
					me->NearTeleportTo(x, y, z, o - G3D::halfPi());
					me->GetMotionMaster()->Clear();
					me->GetMotionMaster()->MoveIdle();
					me->SetTarget(0);
					DoCast(SPELL_TOURBILOL);
					//6s après (= à la fin du tourbilol, JUMP + AOE)
					events.ScheduleEvent(EVENT_JUMP_TO_PLAYER, 6100, 0, PHASE_TWO);
					
				}
					break;

				case EVENT_JUMP_TO_PLAYER:
				{
				   if (!targetForJump)
				   {
					 events.ScheduleEvent(EVENT_SELECT_PLAYER, 0, 0, PHASE_TWO);
					 break;
				   }
				   float dist = me->GetDistance(targetForJump->GetPosition());
				   float vZ = 20.f;
				   float time = (dist / vZ);
				   me->SetTarget(targetForJump->GetGUID());
				   me->JumpTo(targetForJump, vZ);

					events.ScheduleEvent(EVENT_BOSS_AOE, time, 0, PHASE_TWO);
					if (targetForJump->HasAura(SPELL_AURA_P2))
						targetForJump->RemoveAura(SPELL_AURA_P2);
				}
					break;

				case EVENT_BOSS_AOE:
					DoCast(SPELL_AOE_AFTER_JUMP);
					events.ScheduleEvent(EVENT_ADD_WEAKNESS_AURA, 6000, 0, PHASE_TWO);
					break;
					
				case EVENT_ADD_WEAKNESS_AURA:
				{
					me->AddAura(SPELL_STUN_AND_WEAK, me);
					events.AddPhase(PHASE_ONE);
					events.ScheduleEvent(EVENT_SUMMON_ADDS, 10000, 0, PHASE_ONE);
				}
					break;

				case EVENT_ENRAGE:
					Talk(SAY_ENRAGE);
					me->AddAura(SPELL_ENRAGE, me);
					break;
				}
			}

			DoMeleeAttackIfReady();

		}

	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new boss_ZG1AI(creature);
	}

};

class script_jumper : public CreatureScript
{

public: //Constructeur

	script_jumper() : CreatureScript("script_jumper"){}

public: //Override

	struct creaJumperAI : public BossAI
	{

		Unit* lastTarget;
		creaJumperAI(Creature* creature) : BossAI(creature, 0), lastTarget(nullptr)
		{}

		void Reset() override
		{
			UnitAI::Reset();
			lastTarget = nullptr;
		}

		void EnterCombat(Unit* who) override
		{
			events.ScheduleEvent(EVENT_ADD_JUMP_TO_PLAYER, 1000, 0, PHASE_ALL);
		}

		void UpdateAI(uint32 diff) override
		{
			events.Update(diff);

			while (uint32 eId = events.ExecuteEvent())
			{
				switch (eId)
				{
				case EVENT_ADD_JUMP_TO_PLAYER:
				{

					Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0.f, 180.f, true);
					if (target != lastTarget)
					{
						lastTarget = target;
						float dist = me->GetDistance(target->GetPosition());
						float vZ = 5.f;
						float time = (dist / vZ);
						me->getThreatManager().clearReferences();
						me->SetInCombatWithZone();
						me->getThreatManager().addThreat(target, std::numeric_limits<float>::max());
						me->GetMotionMaster()->Clear(true);
						me->GetMotionMaster()->MoveChase(target);
						me->TauntApply(target);

						me->JumpTo(target, vZ);
						events.ScheduleEvent(EVENT_ADD_CAST_STUN, time, 0, PHASE_ALL);
					}
					else{
						events.ScheduleEvent(EVENT_ADD_CAST_STUN, 1000, 0, PHASE_ALL);
					}
					break;
				}
				case EVENT_ADD_CAST_STUN:
					if (lastTarget)
						me->AddAura(SPELL_STUN, lastTarget);
					events.ScheduleEvent(EVENT_ADD_JUMP_TO_PLAYER, 1500, 0, PHASE_ALL);
					break;
				}
			}

			DoMeleeAttackIfReady();


		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new creaJumperAI(creature);
	}

}; 


class script_healer : public CreatureScript
{
public:

	script_healer() : CreatureScript("script_healer"){}

public:

	struct creaHealerAI : public BossAI
	{

		creaHealerAI(Creature* creature) : BossAI(creature, 0){}

		void EnterCombat(Unit* /*who*/) override
		{
			_EnterCombat();
			events.ScheduleEvent(EVENT_HEAL_FRIENDLY_UNIT, 0, 0, PHASE_ALL);
		}

		void UpdateAI(uint32 diff)
		{

			if (!UpdateVictim())
				return;

			events.Update(diff);

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_HEAL_FRIENDLY_UNIT:
					Unit* boss = me->GetCharmer();
					me->CastSpell(boss, SPELL_HEAL, true);

					events.ScheduleEvent(EVENT_HEAL_FRIENDLY_UNIT, 3000, 0, PHASE_ALL);
				}
			}

			DoSpellAttackIfReady(SPELL_SHADOW_BOLT);

		}

	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new creaHealerAI(creature);
	}
};

void AddSC_script_boss_zg1()
{
	new script_boss_zg1();
	new script_jumper();
	new script_healer();
}