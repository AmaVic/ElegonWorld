
#include "ScriptMgr.h"

#define TOWER_A_ENTRY 200010
#define TOWER_H_ENTRY 200011
#define CASTER_A_ENTRY 200020
#define CASTER_H_ENTRY 2000002
#define NEXUS_A_ENTRY 200012
#define NEXUS_H_ENTRY 200013

#define TOWER_SPELL 38935
#define CASTER_SPELL 13438

#define CAMOUFLAGE_DETECT_SPELL 28496

class script_cac_icw : public CreatureScript
{

public: //Constructeur
	script_cac_icw() : CreatureScript("script_cac_icw"){}

	struct cacIcwAI : public CreatureAI
	{
		cacIcwAI(Creature* creature) : CreatureAI(creature){}

		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
				return;

			if (sObjectAccessor->FindUnit(me->GetTarget()) != NULL)
			{
				if (sObjectAccessor->FindUnit(me->GetTarget())->GetDistance(me->GetPosition()) > 20.f)
					me->SetTarget(0);
			}


			uint32 entry = me->GetEntry();

			if (!me->HasAura(CAMOUFLAGE_DETECT_SPELL))
				me->AddAura(CAMOUFLAGE_DETECT_SPELL, me);

			//On recherche les cibles à portée (joueur/sbires)
			Unit* nearestUnit = me->SelectNearestTarget(46.f, false);
			Player* nearestPlayer = me->SelectNearestPlayer(46.f);


			//Si le joueur est allié, on ne l'attaque pas
			if (nearestPlayer)
			{
				if (me->IsFriendlyTo(nearestPlayer))
				{
					nearestPlayer = NULL;
				}
			}


			//On définit la cible
			if (!nearestUnit && !nearestPlayer)
				return;

			if (nearestUnit && !nearestPlayer)
			{
				me->SetTarget(nearestUnit->GetGUID());
			}
			else if (nearestUnit && nearestPlayer && nearestUnit->ToCreature())
			{
				me->SetTarget(nearestUnit->GetGUID());
			}
			else if (nearestUnit && nearestPlayer && nearestUnit->ToPlayer() == nearestPlayer)
			{
				me->SetTarget(nearestPlayer->GetGUID());
			}

			DoSpellAttackIfReady(TOWER_SPELL);


		}
	};

	CreatureAI * GetAI(Creature*  creature) const override
	{
		return new cacIcwAI(creature);
	}

};


class script_sbires_cast : CreatureScript {

public:

	script_sbires_cast() : CreatureScript("script_sbires_cast") {}

	struct sbiresICWAI : public CreatureAI
	{
		sbiresICWAI(Creature* creature) : CreatureAI(creature){}

		void JustDied(Unit* /*killer*/) override
		{
			me->DespawnOrUnsummon();
			me->RemoveFromWorld();
			me->RemoveFromGrid();
		}

		void UpdateAI(uint32 diff) override
		{
			DoSpellAttackIfReady(CASTER_SPELL);
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new sbiresICWAI(creature);
	}
};

class script_sbires_cac : CreatureScript
{
public:

	script_sbires_cac() : CreatureScript("script_sbires_cac"){}

	struct sbiresCastICWAI : public CreatureAI
	{
		sbiresCastICWAI(Creature* creature) : CreatureAI(creature) {}

		void JustDied(Unit* /*killer*/) override
		{
			me->DespawnOrUnsummon();
			me->RemoveFromWorld();
			me->RemoveFromGrid();
		}

		void UpdateAI(uint32 diff) override
		{
			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new sbiresCastICWAI(creature);
	}
};

/*

class script_cac_icw : public CreatureScript
{
public:
	//Constructor
	script_cac_icw() : CreatureScript("script_cac_icw")
	{

	}

public:

	struct cacIcwAI : public CreatureAI
	{
		cacIcwAI(Creature* creature) : CreatureAI(creature)
		{}
		void UpdateAI(uint32 diff) override
		{
			
			uint32 entry = me->GetEntry();
			if (entry == TOWER_A_ENTRY || entry == TOWER_H_ENTRY)
			{
				if (!me->HasAura(CAMOUFLAGE_DETECT_SPELL))
					me->AddAura(CAMOUFLAGE_DETECT_SPELL, me);
			}

			if (!UpdateVictim())
				return;

			if (entry == NEXUS_H_ENTRY || entry == NEXUS_A_ENTRY)
				return;

			Unit*     nearestCreature = me->SelectNearestHostileUnitInAggroRange();
			Player*   nearestPlayer = me->SelectNearestPlayer(10.f);
			//Si le joueur est allié, on ne l'attaque pas
			if (me->IsFriendlyTo(nearestPlayer))
			{
				nearestPlayer = NULL; 
			}

			if (!nearestCreature && !nearestPlayer)
				return;


			std::function<void(Unit*)> attack = [=](Unit* target)
			{
				uint32 entry = me->GetEntry();
				if (entry == TOWER_A_ENTRY || entry == TOWER_H_ENTRY || entry == CASTER_A_ENTRY || entry == CASTER_H_ENTRY)
				{
					me->Attack(target, false);
				}
				else me->Attack(target, true);

			};

			bool canAttackCreature = false;
			bool canAttackPlayer = false;

			if (nearestCreature && nearestPlayer)
			{
				canAttackCreature = true;
			}


			if (!nearestCreature && nearestPlayer)
			{
				if (!me->IsFriendlyTo(nearestPlayer))
				{
					canAttackPlayer = true;
				}
			}


			if (nearestCreature)
			{
				if (!me->IsFriendlyTo(nearestCreature))
					canAttackCreature = true;
			}

			if (canAttackCreature && canAttackPlayer)
				attack(nearestCreature);
			else if (canAttackCreature && !canAttackPlayer)
				attack(nearestCreature);
			else if (!canAttackCreature && canAttackPlayer)
				attack(nearestPlayer);

			if (entry == TOWER_A_ENTRY || entry == TOWER_H_ENTRY || entry == CASTER_A_ENTRY || entry == CASTER_H_ENTRY)
			{
				DoSpellAttackIfReady(TOWER_SPELL);
			}
			else {
				DoMeleeAttackIfReady();
			}

		}

	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new cacIcwAI(creature);
	}

	void OnUpdate(Creature* creature, uint32 diff)
	{
		/*

		if (creature->IsInCombat())
			return;

		Unit*     nearestCreature = creature->SelectNearestHostileUnitInAggroRange();
		Player*   nearestPlayer = creature->SelectNearestPlayer(10.f);

		if (!nearestCreature && !nearestPlayer)
			return;


		std::function<void(Unit*)> attack = [=](Unit* target)
		{
			uint32 entry = creature->GetEntry();
			if (entry == TOWER_A_ENTRY || entry == TOWER_H_ENTRY || entry == CASTER_A_ENTRY || entry == CASTER_H_ENTRY)
			{
				creature->GetAI()->AttackStart(target);
			}
			else creature->GetAI()->AttackStart(target);

		};

		bool canAttackCreature = false;
		bool canAttackPlayer = false;

		if (nearestCreature && nearestPlayer)
		{
			canAttackCreature = true;
		}
		

		if (!nearestCreature && nearestPlayer)
		{
			if (!creature->IsFriendlyTo(nearestPlayer))
			{
				canAttackPlayer = true;
			}
		}


		if (nearestCreature)
		{
			if (!creature->IsFriendlyTo(nearestCreature))
				canAttackCreature = true;
		}

		if (canAttackCreature && canAttackPlayer)
			attack(nearestCreature);
		else if (canAttackCreature && !canAttackPlayer)
			attack(nearestCreature);
		else if (!canAttackCreature && canAttackPlayer)
			attack(nearestPlayer);
			
	}
}; */

void AddSC_script_cac_icw()
{
	new script_cac_icw();
	new script_sbires_cac();
	new script_sbires_cast();
}