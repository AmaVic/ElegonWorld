
#include "ScriptMgr.h"
#include "Player.h"

#define SPELL_FEAR 31970	
#define SPELL_STUN 59705
#define SPELL_AOE  65333
#define BUFF_DMG   41107
#define BUFF_HP    12975
#define BUFF_SPEED 2983


class script_item_fear : public ItemScript
{
public:
	script_item_fear() : ItemScript("script_item_fear") { }

	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}
		player->DestroyItemCount(350000, 1, true);
		player->CastSpell(player, SPELL_FEAR);
		return true;

	}

};

class script_item_stun : public ItemScript
{
public:
	script_item_stun() : ItemScript("script_item_stun") { }


	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}
		player->CastSpell(player, SPELL_STUN);
		player->DestroyItemCount(350001, 1, true);

		return true;
	}

};

class script_item_aoe : public ItemScript
{
public:

	script_item_aoe() : ItemScript("script_item_aoe"){}

	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}

		player->CastSpell(player, SPELL_AOE);
		player->DestroyItemCount(350002, 1, true);

		return true;
	}

};

class script_item_buff_damage : public ItemScript
{
public:

	script_item_buff_damage() : ItemScript("script_item_buff_damage"){}

	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}

		player->AddAura(BUFF_DMG, player);
		player->DestroyItemCount(350003, 1, true);

		return true;
	}

};

class script_item_buff_hp : public ItemScript
{
public:

	script_item_buff_hp() : ItemScript("script_item_buff_hp"){}


	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}

		player->CastSpell(player, BUFF_HP);
		player->DestroyItemCount(350005, 1, true);

		return true;
	}

};

class script_item_buff_speed : public ItemScript
{
public:

	script_item_buff_speed() : ItemScript("script_item_buff_speed"){}


	// Called when a player uses the item.
	bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
	{
		if (player->InBattleground() || player->InArena() || player->GetMap()->IsDungeon() || player->GetMap()->IsRaid())
		{
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser ca ici.");
			return false;
		}

		player->AddAura(BUFF_SPEED, player);
		player->DestroyItemCount(350004, 1, true);

		return true;
	}

};

void AddSC_scripts_items_campzg()
{
	new script_item_fear();
	new script_item_stun();
	new script_item_aoe();
	new script_item_buff_damage();
	new script_item_buff_speed();
	new script_item_buff_hp();
}