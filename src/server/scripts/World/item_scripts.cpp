/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/* ContentData
item_nether_wraith_beacon(i31742)   Summons creatures for quest Becoming a Spellfire Tailor (q10832)
item_flying_machine(i34060, i34061)  Engineering crafted flying machines
item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
item_only_for_flight                Items which should only useable while flying
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Player.h"
#include <vector>
#include <string>
using namespace std;

/*#####
# item_only_for_flight
#####*/

enum OnlyForFlight
{
    SPELL_ARCANE_CHARGES    = 45072
};

class item_only_for_flight : public ItemScript
{
public:
    item_only_for_flight() : ItemScript("item_only_for_flight") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        uint32 itemId = item->GetEntry();
        bool disabled = false;

        //for special scripts
        switch (itemId)
        {
           case 24538:
			   if (player->GetAreaId() != 3628)
				   player->CastSpell(player, 20000);
                    disabled = true;
                    break;
           case 34489:
                if (player->GetZoneId() != 4080)
                    disabled = true;
                    break;
           case 34475:
                if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_ARCANE_CHARGES))
                    Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_ON_GROUND);
                    break;
        }

        // allow use in flight only
        if (player->IsInFlight() && !disabled)
            return false;

        // error
        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, NULL);
        return true;
    }
};
class item_cast_pourpe : public ItemScript
{
private : 
public:
	item_cast_pourpe() : ItemScript("item_cast_pourpe") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		player->KilledMonsterCredit(3500);
		Creature * PortalBeam = player->FindNearestCreature(27306, 100.f);
			player->CastSpell(PortalBeam, 48310);
					player->GetSession()->GetPlayer()->DealDamage(player, 200000, NULL, DIRECT_DAMAGE);
						player->GetSession()->SendAreaTriggerMessage("Attention vos points de vie on recus un grand nombre de degats !");
		return true;
	}
};
/*#####
# item_nether_wraith_beacon
#####*/

class item_nether_wraith_beacon : public ItemScript
{
public:
    item_nether_wraith_beacon() : ItemScript("item_nether_wraith_beacon") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
    {
        if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()+20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);

            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()-20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
        }
        return false;
    }
};

/*#####
# item_gor_dreks_ointment
#####*/

class item_gor_dreks_ointment : public ItemScript
{
public:
    item_gor_dreks_ointment() : ItemScript("item_gor_dreks_ointment") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
    {
        if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT &&
            targets.GetUnitTarget()->GetEntry() == 20748 && !targets.GetUnitTarget()->HasAura(32578))
            return false;

        player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, NULL);
        return true;
    }
};

/*#####
# item_incendiary_explosives
#####*/

class item_incendiary_explosives : public ItemScript
{
public:
    item_incendiary_explosives() : ItemScript("item_incendiary_explosives") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/) override
    {
        if (player->FindNearestCreature(26248, 15) || player->FindNearestCreature(26249, 15))
            return false;
        else
        {
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
            return true;
        }
    }
};

/*#####
# item_mysterious_egg
#####*/

class item_mysterious_egg : public ItemScript
{
public:
    item_mysterious_egg() : ItemScript("item_mysterious_egg") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) override
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 39883, 1); // Cracked Egg
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 39883, true, Item::GenerateItemRandomPropertyId(39883));
        return true;
    }
};
class item_voyager : public ItemScript
{
public:
	item_voyager() : ItemScript("item_voyager") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets) override
	{

		if (player->GetQuestStatus(900950) == QUEST_STATE_COMPLETE || player->GetQuestStatus(900951) == QUEST_STATE_COMPLETE || player->GetQuestStatus(900952) == QUEST_STATE_COMPLETE ||
			player->GetQuestStatus(900953) == QUEST_STATE_COMPLETE || player->GetQuestStatus(900954) == QUEST_STATE_COMPLETE || player->GetQuestStatus(900955) == QUEST_STATE_COMPLETE ||
			player->GetQuestStatus(900956) == QUEST_STATE_COMPLETE)
		{
			player->GetSession()->SendAreaTriggerMessage("Vous avez deja une quete en cours, vous ne pouvez pas en prendre une en plus.");
			return true;
		}
		if (player->HasAura(68852, player->GetGUID()))
		{
			player->GetSession()->SendAreaTriggerMessage("Vous devez attendre au moins 4 heures avant de recommencer une quete... Reposez vous !");
			return true;
		}
		else {
			if (Creature * creature = player->FindNearestCreature(67784, 1.f))
			{
				uint32 rand_quest(urand(900950, 900954));
				Quest const* quest = sObjectMgr->GetQuestTemplate(rand_quest);
				Object * giver = creature->ToCreature();
				player->AddQuest(quest, giver);
				player->SetQuestStatus(rand_quest, QUEST_STATUS_COMPLETE);
			}
			else {
				player->GetSession()->SendAreaTriggerMessage("Vous devez etre a proximite du Messager Tombemartel pour prendre une lettre de voyage...");
			}
			
		}

	}
};
/*#####
# item_disgusting_jar
#####*/

class item_disgusting_jar : public ItemScript
{
public:
    item_disgusting_jar() : ItemScript("item_disgusting_jar") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/) override
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 44718, 1); // Ripe Disgusting Jar
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 44718, true, Item::GenerateItemRandomPropertyId(44718));

        return true;
    }
};

/*#####
# item_pile_fake_furs
#####*/

enum PileFakeFur
{
    GO_CARIBOU_TRAP_1                                      = 187982,
    GO_CARIBOU_TRAP_2                                      = 187995,
    GO_CARIBOU_TRAP_3                                      = 187996,
    GO_CARIBOU_TRAP_4                                      = 187997,
    GO_CARIBOU_TRAP_5                                      = 187998,
    GO_CARIBOU_TRAP_6                                      = 187999,
    GO_CARIBOU_TRAP_7                                      = 188000,
    GO_CARIBOU_TRAP_8                                      = 188001,
    GO_CARIBOU_TRAP_9                                      = 188002,
    GO_CARIBOU_TRAP_10                                     = 188003,
    GO_CARIBOU_TRAP_11                                     = 188004,
    GO_CARIBOU_TRAP_12                                     = 188005,
    GO_CARIBOU_TRAP_13                                     = 188006,
    GO_CARIBOU_TRAP_14                                     = 188007,
    GO_CARIBOU_TRAP_15                                     = 188008,
    GO_HIGH_QUALITY_FUR                                    = 187983,
    NPC_NESINGWARY_TRAPPER                                 = 25835
};

#define CaribouTrapsNum 15
const uint32 CaribouTraps[CaribouTrapsNum] =
{
    GO_CARIBOU_TRAP_1, GO_CARIBOU_TRAP_2, GO_CARIBOU_TRAP_3, GO_CARIBOU_TRAP_4, GO_CARIBOU_TRAP_5,
    GO_CARIBOU_TRAP_6, GO_CARIBOU_TRAP_7, GO_CARIBOU_TRAP_8, GO_CARIBOU_TRAP_9, GO_CARIBOU_TRAP_10,
    GO_CARIBOU_TRAP_11, GO_CARIBOU_TRAP_12, GO_CARIBOU_TRAP_13, GO_CARIBOU_TRAP_14, GO_CARIBOU_TRAP_15,
};

class item_pile_fake_furs : public ItemScript
{
public:
    item_pile_fake_furs() : ItemScript("item_pile_fake_furs") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const & /*targets*/) override
    {
        GameObject* go = NULL;
        for (uint8 i = 0; i < CaribouTrapsNum; ++i)
        {
            go = player->FindNearestGameObject(CaribouTraps[i], 5.0f);
            if (go)
                break;
        }

        if (!go)
            return false;

        if (go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, true) || go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, false) || go->FindNearestGameObject(GO_HIGH_QUALITY_FUR, 2.0f))
            return true;

        float x, y, z;
        go->GetClosePoint(x, y, z, go->GetObjectSize() / 3, 7.0f);
        go->SummonGameObject(GO_HIGH_QUALITY_FUR, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 0, 0, 0, 0, 0, 1000);
        if (TempSummon* summon = player->SummonCreature(NPC_NESINGWARY_TRAPPER, x, y, z, go->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 1000))
        {
            summon->SetVisible(false);
            summon->SetReactState(REACT_PASSIVE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }
        return false;
    }
};

/*#####
# item_petrov_cluster_bombs
#####*/

enum PetrovClusterBombs
{
    SPELL_PETROV_BOMB           = 42406,
    AREA_ID_SHATTERED_STRAITS   = 4064,
    ZONE_ID_HOWLING             = 495
};

class item_petrov_cluster_bombs : public ItemScript
{
public:
    item_petrov_cluster_bombs() : ItemScript("item_petrov_cluster_bombs") { }

    bool OnUse(Player* player, Item* item, const SpellCastTargets & /*targets*/) override
    {
        if (player->GetZoneId() != ZONE_ID_HOWLING)
            return false;

        if (!player->GetTransport() || player->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
        {
            player->SendEquipError(EQUIP_ERR_NONE, item, NULL);

            if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_PETROV_BOMB))
                Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_HERE);

            return true;
        }

        return false;
    }
};

/*######
# item_dehta_trap_smasher
# For quest 11876, Help Those That Cannot Help Themselves
######*/
enum HelpThemselves
{
    QUEST_CANNOT_HELP_THEMSELVES                  =  11876,
    NPC_TRAPPED_MAMMOTH_CALF                      =  25850,
    GO_MAMMOTH_TRAP_1                             = 188022,
    GO_MAMMOTH_TRAP_2                             = 188024,
    GO_MAMMOTH_TRAP_3                             = 188025,
    GO_MAMMOTH_TRAP_4                             = 188026,
    GO_MAMMOTH_TRAP_5                             = 188027,
    GO_MAMMOTH_TRAP_6                             = 188028,
    GO_MAMMOTH_TRAP_7                             = 188029,
    GO_MAMMOTH_TRAP_8                             = 188030,
    GO_MAMMOTH_TRAP_9                             = 188031,
    GO_MAMMOTH_TRAP_10                            = 188032,
    GO_MAMMOTH_TRAP_11                            = 188033,
    GO_MAMMOTH_TRAP_12                            = 188034,
    GO_MAMMOTH_TRAP_13                            = 188035,
    GO_MAMMOTH_TRAP_14                            = 188036,
    GO_MAMMOTH_TRAP_15                            = 188037,
    GO_MAMMOTH_TRAP_16                            = 188038,
    GO_MAMMOTH_TRAP_17                            = 188039,
    GO_MAMMOTH_TRAP_18                            = 188040,
    GO_MAMMOTH_TRAP_19                            = 188041,
    GO_MAMMOTH_TRAP_20                            = 188042,
    GO_MAMMOTH_TRAP_21                            = 188043,
    GO_MAMMOTH_TRAP_22                            = 188044,
};

#define MammothTrapsNum 22
const uint32 MammothTraps[MammothTrapsNum] =
{
    GO_MAMMOTH_TRAP_1, GO_MAMMOTH_TRAP_2, GO_MAMMOTH_TRAP_3, GO_MAMMOTH_TRAP_4, GO_MAMMOTH_TRAP_5,
    GO_MAMMOTH_TRAP_6, GO_MAMMOTH_TRAP_7, GO_MAMMOTH_TRAP_8, GO_MAMMOTH_TRAP_9, GO_MAMMOTH_TRAP_10,
    GO_MAMMOTH_TRAP_11, GO_MAMMOTH_TRAP_12, GO_MAMMOTH_TRAP_13, GO_MAMMOTH_TRAP_14, GO_MAMMOTH_TRAP_15,
    GO_MAMMOTH_TRAP_16, GO_MAMMOTH_TRAP_17, GO_MAMMOTH_TRAP_18, GO_MAMMOTH_TRAP_19, GO_MAMMOTH_TRAP_20,
    GO_MAMMOTH_TRAP_21, GO_MAMMOTH_TRAP_22
};

class item_dehta_trap_smasher : public ItemScript
{
public:
    item_dehta_trap_smasher() : ItemScript("item_dehta_trap_smasher") { }

    bool OnUse(Player* player, Item* /*item*/, const SpellCastTargets & /*targets*/) override
    {
        if (player->GetQuestStatus(QUEST_CANNOT_HELP_THEMSELVES) != QUEST_STATUS_INCOMPLETE)
            return false;

      
  Creature* pMammoth = player->FindNearestCreature(NPC_TRAPPED_MAMMOTH_CALF, 5.0f);
        if (!pMammoth)
            return false;
        GameObject* pTrap = NULL;
        for (uint8 i = 0; i < MammothTrapsNum; ++i)
        {
            pTrap = player->FindNearestGameObject(MammothTraps[i], 11.0f);
            if (pTrap)
            {
                pMammoth->AI()->DoAction(1);
                pTrap->SetGoState(GO_STATE_READY);
                player->KilledMonsterCredit(NPC_TRAPPED_MAMMOTH_CALF, 0);
                return true;
            }
        }
        return false;
    }
};

enum TheEmissary
{
    QUEST_THE_EMISSARY      =   11626,
    NPC_LEVIROTH            =   26452
};

class item_trident_of_nazjan : public ItemScript
{
public:
    item_trident_of_nazjan() : ItemScript("item_Trident_of_Nazjan") { }

    bool OnUse(Player* player, Item* item, const SpellCastTargets & /*targets*/) override
    {
        if (player->GetQuestStatus(QUEST_THE_EMISSARY) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* pLeviroth = player->FindNearestCreature(NPC_LEVIROTH, 10.0f)) // spell range
            {
                pLeviroth->AI()->AttackStart(player);
                return false;
            } else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        } else
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, NULL);
        return true;
    }
};

enum CapturedFrog
{
    QUEST_THE_PERFECT_SPIES      = 25444,
    NPC_VANIRAS_SENTRY_TOTEM     = 40187
};

class item_captured_frog : public ItemScript
{
public:
    item_captured_frog() : ItemScript("item_captured_frog") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (player->GetQuestStatus(QUEST_THE_PERFECT_SPIES) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->FindNearestCreature(NPC_VANIRAS_SENTRY_TOTEM, 10.0f))
                return false;
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        else
            player->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, item, NULL);
        return true;
    }
};

class item_scie_vapeur : public ItemScript
{
public:
	item_scie_vapeur() : ItemScript("item_scie_vapeur") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
	
		if (GameObject * tree = player->FindNearestGameObject(181824, 2.0f)) {

			tree->Delete();
			player->AddItem(4470, urand(1, 10));
			player->HandleEmoteCommand(30);
		}
		else {

			player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);

		}

		return true;
	}
};

class item_tree_spawn : public ItemScript
{
public:
	item_tree_spawn() : ItemScript("item_tree_spawn") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->GetQuestStatus(200217) == QUEST_STATUS_INCOMPLETE) {
			if (player->GetAreaId() == 15) {

				if (GameObject * tree = player->FindNearestGameObject(353500, 20.0f)) {

					player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas le mettre trop pres d'un autre arbres");
				}
				else {
					player->SummonGameObject(353500, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), 0, 0, 0, 0, 15);
					player->KilledMonsterCredit(3000);
				}

			}
			else {
				player->GetSession()->SendAreaTriggerMessage("Vous n'etes pas dans la bonne zone !");
			}
		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez plus faire ca !");

		}
			return true;
	}
};

class item_Cuisse: public ItemScript
{
public:
	item_Cuisse() : ItemScript("item_Cuisse") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->GetQuestStatus(200218) == QUEST_STATUS_INCOMPLETE) {
			if (Creature* drake = player->FindNearestCreature(30500, 0.5f)) {

				player->DestroyItemCount(332020, 1, true);
				player->KilledMonsterCredit(3001);

			}
			else {

				player->GetSession()->SendAreaTriggerMessage("Vous devez etre devant un proto-drake");
			}
		}


		return true;
	}
};

class pil_face_item : public ItemScript
{
public:
	pil_face_item() : ItemScript("pil_face_item") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{

		switch (urand(0, 1)) {

		case 0:
			player->DestroyItemCount(437050, 1, true);
			player->SetHonorPoints(player->GetHonorPoints() + 75000);
			player->SetArenaPoints(player->GetArenaPoints() + 1000);
			break;
		case 1:
			player->DestroyItemCount(437050, 1, true);
			player->AddItem(430407, 10);
			break;

		}



		return true;
	}
};
class item_peche : public ItemScript
{
public:
	item_peche() : ItemScript("item_pêche") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->GetQuestStatus(250004) == QUEST_STATUS_INCOMPLETE) {


			if (Creature* drake = player->FindNearestCreature(12999, 2.0f)) {

				player->AddItem(35285, urand(10, 100));

			}
			else {

				player->GetSession()->SendAreaTriggerMessage("Vous devez etre dans le lac de Grisgueule");
			}

		}
		return true;
	}
};

class item_engrais : public ItemScript
{
public:
	item_engrais() : ItemScript("item_engrais") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT in_progress, in_progress_1, in_progress_2, in_progress_3, progression, progression_1, progression_2, progression_3 FROM zone_player WHERE guid = %u", player->GetGUID());
		
		Field *fields = result->Fetch();
		uint32 in_progress = fields[0].GetUInt32();
		uint32 in_progress_1 = fields[1].GetUInt32();
		uint32 in_progress_2 = fields[2].GetUInt32();
		uint32 in_progress_3 = fields[3].GetUInt32();
		uint32 progression = fields[4].GetUInt32();
		uint32 progression_1 = fields[5].GetUInt32();
		uint32 progression_2 = fields[6].GetUInt32();
		uint32 progression_3 = fields[7].GetUInt32();


		
		if (in_progress == 1 && progression > 50) {
			CharacterDatabase.PQuery("UPDATE zone_player SET progression = progression + 25 WHERE guid = %u", player->GetGUID());
			player->DestroyItemCount(73904, 1, true);

		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela quand votre progression est plus haut que 50% !");
		}
		if (in_progress_1 == 1 && progression_1 < 50) {
			CharacterDatabase.PQuery("UPDATE zone_player SET progression_1 = progression + 25 WHERE guid = %u", player->GetGUID());
			player->DestroyItemCount(73904, 1, true);

		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela quand votre progression est plus haut que 50% !");
		}
 		if (in_progress_2 == 1 && progression_2 < 50) {
			CharacterDatabase.PQuery("UPDATE zone_player SET progression_2 = progression + 25 WHERE guid = %u", player->GetGUID());
			player->DestroyItemCount(73904, 1, true);

		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela quand votre progression est plus haut que 50%  !");
		}
		if (in_progress_3 == 1 && progression_3 > 50) {
			CharacterDatabase.PQuery("UPDATE zone_player SET progression_3 = progression + 25 WHERE guid = %u", player->GetGUID());
			player->DestroyItemCount(73904, 1, true);
		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela quand votre progression est plus haut que 50%  !");
		}
		return true;
	}
};
class item_teleport_phase : public ItemScript
{
public:
	item_teleport_phase() : ItemScript("item_teleport_phase") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT x, y, z, o FROM house_buy WHERE owner = %u", player->GetGUID());

		Field *fields = result->Fetch();
		float x = fields[0].GetFloat();
		float y = fields[1].GetFloat();
		float z = fields[2].GetFloat();
		float o = fields[3].GetFloat();

		player->TeleportTo(560, x, y, z, o);
		return true;
	}
};

class item_livre : public ItemScript
{
public:
	item_livre() : ItemScript("item_livre") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->FindNearestCreature(190010, 2.f, true)) {
			player->CastSpell(player, 35426);
		}
		return true;
	}
};
class item_stockage : public ItemScript
{
public:
	item_stockage() : ItemScript("item_stockage") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->FindNearestGameObject(178785, 1.0f)) {
			player->DestroyItemCount(430400, 1, true);
			player->KilledMonsterCredit(5000);
		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous devez etres dans la tour en bois pour la gestion des stocks.");
		}
		return true;
	}
};
class item_caisse_quest : public ItemScript
{
public:
	item_caisse_quest() : ItemScript("item_caisse_quest") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->HasItemCount(110000, 1, true)) {
			player->DestroyItemCount(864700, 1, true);
			player->AddItem(864701, 1);
			player->AddItem(110000, 1);
		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas la cle pour ouvrir ce coffre.");
		}
		return true;
	}
};
class Item_fire_ecarlate : public ItemScript
{
public:
	Item_fire_ecarlate() : ItemScript("Item_fire_ecarlate") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (Creature * firecreature = player->FindNearestCreature(12999, 9.f))
		{
			player->SummonGameObject(193377, firecreature->GetPositionX(), firecreature->GetPositionY(), firecreature->GetPositionZ(), 0, 0, 0, 0, 0, 30);
			firecreature->DespawnOrUnsummon();
			player->KilledMonsterCredit(5000);
			player->CastSpell(firecreature, 19823);
		}
		else {
			player->GetSession()->SendAreaTriggerMessage("Vous devez etres à proximite du campement de Halte de la vigie.");

		}

		return true;
	}
};
class item_titre : public ItemScript
{
public:
	item_titre() : ItemScript("item_titre") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(182);
		player->SetTitle(titleInfo);
		player->DestroyItemCount(347151, 1, true);
		return true;
	}
};
class Item_tambour : public ItemScript
{
public:
	Item_tambour() : ItemScript("Item_tambour") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		player->AddAura(35475, player);
		return true;
	}

};
class item_assasinat : public ItemScript
{
public:
	item_assasinat() : ItemScript("item_assasinat") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{

		Creature * creature = player->FindNearestCreature(263000, 5.f);

		if (player->FindNearestCreature(67982, 100.f))
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela.");
			return false;
		}

		if (!creature) // Si la créature n'est pas là
		{
			player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de creature eligible au meurte pres de vous. Les cibles sont marquees d'un indicateur");
			return false;
		}
		else
		{
			player->SummonCreature(67982, creature->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 10000);
		}

	}
};
class item_morph : public ItemScript
{
public:
	item_morph() : ItemScript("item_morph") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (item->GetEntry() == 332821)
		{
			player->SetDisplayId(24991); // Valkyr
		}
		if (item->GetEntry() == 332822)
		{
			player->SetDisplayId(25325); // Roi-liche
		}
		if (item->GetEntry() == 332823)
		{
			player->SetDisplayId(17370); // Thrall
		}
		if (item->GetEntry() == 332824)
		{
			player->SetDisplayId(27016);  // Sylvanas
		}
		if (item->GetEntry() == 332825)
		{
			player->SetDisplayId(24829); // Abomination
		}
		if (item->GetEntry() == 332826)
		{
			player->SetDisplayId(26942); // Zombie
		}
		if (item->GetEntry() == 332827)
		{
			player->SetDisplayId(25664); //  Geist
		}
		if (item->GetEntry() == 332828)
		{
			player->SetDisplayId(21526); //  Illidan
		}
		if (item->GetEntry() == 332829)
		{
			player->SetDisplayId(1132); // Marcheur du vide
		}
		if (item->GetEntry() == 332830)
		{
			player->SetDisplayId(1060); // Vache
		}
		if (item->GetEntry() == 332831)
		{
			player->SetDisplayId(1405); // Elementaire de feu
		}
		if (item->GetEntry() == 332832)
		{
			player->SetDisplayId(20023); // Kael'thas
		}
		if (item->GetEntry() == 332833)
		{
			player->SetDisplayId(5965); // Anubisath
		}

		return true;

	}
};

class item_talent : public ItemScript
{
public:
	item_talent() : ItemScript("item_talent") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT max_talent FROM characters WHERE guid = %u", player->GetGUID());
		Field *fields = result->Fetch();
		uint32 max_talent = fields[0].GetUInt32();
		 
		if (max_talent == 11 && player->HasItemCount(430405, 40, true)) {
			player->GetSession()->SendAreaTriggerMessage("Vous avez le nombre maximal de points de talent. Neamoins vous gagner 1000 points d'experiences pour votre maisons.");
			CharacterDatabase.PQuery("UPDATE house_player SET exp = exp + 1000 WHERE guid_player = %u", player->GetGUID());
			player->DestroyItemCount(430405, 40, true);
			return true;
		}

		if (player->HasItemCount(430405, 40, true)) {

			CharacterDatabase.PQuery("UPDATE characters SET max_talent = max_talent + 1 WHERE guid = %u", player->GetGUID());
			player->GetSession()->SendAreaTriggerMessage("Vous avez gagner 1 point de talent");
			player->DestroyItemCount(430405, 40, true);
			player->SetFreeTalentPoints(player->GetFreeTalentPoints() + 1);
			player->SendTalentsInfoData(true);
			player->SendInitialPacketsBeforeAddToMap();


		}
		return true;
	}
};

class item_reward_event : public ItemScript
{
public:
	item_reward_event() : ItemScript("item_reward_event") { }

	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		int itemLevel = player->GetAverageItemLevel();

		if (!itemLevel)
			return true;

		if (!player)
			return true;

		if (!item)
			return true;

		if (itemLevel >= 10 && itemLevel <= 251)
			switch (urand(1, 3)) {
			case 1:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250000, 1);
				break;
			case 2:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250001, 1);

				break;
			case 3:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250002, 1);
				break;
		}

		 if (itemLevel >= 268 && itemLevel <= 276)
			switch (urand(4, 6)) {
			case 4:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250003, 1);
				break;
			case 5:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250005, 1);

				break;
			case 6:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250006, 1);
				break;
		}

		 if (itemLevel >= 276 && itemLevel <= 400)
			switch (urand(7, 9)) {
			case 7:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250007, 1);
				break;
			case 8:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250008, 1);

				break;
			case 9:
				player->DestroyItemCount(430407, 1, true);

				player->AddItem(250009, 1);
				break;
		}
		return true;

	}
};

void SpawnGameObject(Player * player, Item * item, int item_id, int gob_id)
{

		float x = float(player->GetPositionX());
		float y = float(player->GetPositionY());
		float z = float(player->GetPositionZ());
		float o = float(player->GetOrientation());
		Map* map = player->GetMap();

		GameObject* object = new GameObject();
		uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

		object->Create(guidLow, gob_id, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
		player->SummonGameObject(gob_id, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
		object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
		sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
		object->LoadGameObjectFromDB(guidLow, map);
		object->SetRespawnTime(300);
		player->DestroyItemCount(item_id, 1, true);

}
class item_hous : public ItemScript
{
public:
	item_hous() : ItemScript("item_hous") { }


	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->GetZoneId() != 2367) {
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire cela ici");
			return false;

		}
			if (item->GetEntry() == 250017){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 181075, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(181075, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250017, 1, true);
				return true;
			}

			if (item->GetEntry() == 250018){


				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 180334, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(180334, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250018, 1, true);
				return true;

			}

			if (item->GetEntry() == 250019){


				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 195063, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(195063, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250019, 1, true);
				return true;

			}

			if (item->GetEntry() == 250020){


				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 193840, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(193840, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250020, 1, true);
				return true;

			}
			if (item->GetEntry() == 250021){


				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 174849, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(174849, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250021, 1, true);
				return true;

			}
			if (item->GetEntry() == 250022){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 180885, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(180885, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250022, 1, true);
				return true;

			}
			if (item->GetEntry() == 250023){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 148914, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(148914, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250023, 1, true);
				return true;

			}

			if (item->GetEntry() == 250024){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 333, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(333, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250024, 1, true);
				return true;

			}
			if (item->GetEntry() == 250025){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 183602, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(183602, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250025, 1, true);
				return true;

			}
			if (item->GetEntry() == 250026){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 1760, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(1760, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250026, 1, true);
				return true;

			}

			if (item->GetEntry() == 250027){

				float x = float(player->GetPositionX());
				float y = float(player->GetPositionY());
				float z = float(player->GetPositionZ());
				float o = float(player->GetOrientation());
				Map* map = player->GetMap();

				GameObject* object = new GameObject();
				uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

				object->Create(guidLow, 181082, map, player->GetPhaseMask(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY);
				player->SummonGameObject(181082, x, y, z, o, 0, 0, 0.0f, 0.0f, 300);
				object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetGUID());
				sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));
				object->LoadGameObjectFromDB(guidLow, map);
				object->SetRespawnTime(300);
				player->DestroyItemCount(250027, 1, true);
			}
			return false;
	}
};

void AddSC_item_scripts()
{
    new item_only_for_flight();
    new item_nether_wraith_beacon();
    new item_gor_dreks_ointment();
    new item_incendiary_explosives();
    new item_mysterious_egg();
    new item_disgusting_jar();
    new item_pile_fake_furs();
    new item_petrov_cluster_bombs();
    new item_dehta_trap_smasher();
    new item_trident_of_nazjan();
    new item_captured_frog();
	new item_scie_vapeur();
	new item_hous();
	new item_tree_spawn();
	new item_Cuisse();
	new item_teleport_phase();
	new item_stockage();
	new item_peche();
	new item_reward_event();
	new item_talent();
	new item_caisse_quest();
	new item_titre();
	new item_engrais();
	new pil_face_item();
	new item_cast_pourpe();
	new Item_fire_ecarlate();
	new item_morph();
	new item_voyager();
	new item_assasinat();
	new Item_tambour();
}
