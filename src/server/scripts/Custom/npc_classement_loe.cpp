

#include "ScriptMgr.h"

#include <iostream>
#include <iomanip>

class npc_classement_loe : public CreatureScript
{

public:

	npc_classement_loe() : CreatureScript("npc_classement_loe")
	{}

public:

	bool OnGossipHello(Player* player, Creature* creature)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT * FROM loe_ranking ORDER BY victories DESC LIMIT 10");
		if (!result)
		{
			return false;
		}

		int i = 1;
		do
		{
			Field* data = result->Fetch();
			//Récup des données
			uint64 playerGUID = data[0].GetUInt32();
			uint32 victories = data[1].GetInt32();
			uint32 defeats = data[2].GetInt32();

			//Récupération du nom du joueur
			QueryResult nameResult = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid=%u", playerGUID);
			Field* dataName = nameResult->Fetch();
			std::string playerName = dataName[0].GetString();
			std::stringstream strStream;
			strStream << ( (i == 1) ? "|TInterface/ICONS/Achievement_pvp_a_11:20|t Champion " : std::to_string(i) + ".") << "|cff045FB4 Nom: |r" << playerName;

			std::stringstream strStream2;
			strStream2 << "|TInterface/ICONS/Achievement_arena_2v2_2:15|t Victoires: " << std::to_string(victories);

			std::stringstream strStream3;
			strStream3 << "|TInterface/ICONS/Achievement_bg_kill_flag_carrier:15|t Defaites: " << std::to_string(defeats);


			//Affichage du gossip
			player->ADD_GOSSIP_ITEM(2, strStream.str().c_str(), GOSSIP_SENDER_MAIN, 0);
			player->ADD_GOSSIP_ITEM(2, strStream2.str().c_str(), GOSSIP_SENDER_MAIN, 0);
			player->ADD_GOSSIP_ITEM(2, strStream3.str().c_str(), GOSSIP_SENDER_MAIN, 0);

			//Incrémentation
			i++;
		} while (result->NextRow());

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;

	}
};

void AddSC_npc_classement_loe()
{
	new npc_classement_loe();
}

