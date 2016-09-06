
#include "ScriptMgr.h"
#include "RiftHandler.h"

class script_npc_info_rift : public CreatureScript
{
public:

	script_npc_info_rift() : CreatureScript("npc_info_rift") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		std::string riftAreaMsg = "|TInterface/ICONS/Achievement_dungeon_nexus80_10man:20|t Localisation de la faille: " + sRH->GetAreaName();
		std::string underAttackMsg = "Faille en combat: " + std::string((sRH->IsUnderAttack() ? "|cff00FF00 Oui" : "|cffFF0000 Non"));
		player->ADD_GOSSIP_ITEM(0, riftAreaMsg.c_str(), GOSSIP_SENDER_MAIN,0);
		player->ADD_GOSSIP_ITEM(0, underAttackMsg.c_str(), GOSSIP_SENDER_MAIN,0);

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;
	}
};

void AddSC_npc_info_rift()
{
	new script_npc_info_rift();
}