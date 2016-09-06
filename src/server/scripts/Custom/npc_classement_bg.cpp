
#include "BGScore.h"
#include "BattlegroundMgr.h"

class script_npc_classement_bg_all : public CreatureScript
{

public:

	script_npc_classement_bg_all() : CreatureScript("script_npc_classement_bg_all"){}

	bool OnGossipHello(Player* player, Creature* creature)
	{

		//On liste les BG pour lesquelles il existe des données, et si on en a pas, on dit qu'on a pas données
		std::vector<uint32> bg_ids = BGScore::getBGWhereDataExists();
		if (bg_ids.empty())
		{
			creature->MonsterSay("Je n'ai pas d'informations sur les BG pour le moment", LANG_UNIVERSAL, player);
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		//Si on arrive ici, on liste les BG
		for (std::size_t i = 0; i < bg_ids.size(); i++)
		{
			const char* bg_name = sBattlegroundMgr->GetBattlegroundTemplate(BattlegroundTypeId(i))->GetName();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, bg_name, GOSSIP_SENDER_MAIN, i);
		}

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;

	}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
		{
			player->PlayerTalkClass->ClearMenus();

			//Récupération des scores:
			//Action = BG_ID
			std::vector<BGScore> scores = BGScore::getBGScores(action);
			for (std::size_t i = 0; i < scores.size(); i++)
			{
				BGScore score = scores.at(i);
				std::string playername = "inconnu";
				QueryResult result = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid=%u", score.getPlayerGUID());
				if (result)
				{
					Field* fields = result->Fetch();
					playername = fields[0].GetString();
				}

				std::stringstream strstream;
				strstream << ((i == 1) ? "|TInterface/ICONS/Achievement_pvp_a_11:20|t Champion " : std::to_string(i) + ".") << "|cff045FB4 Nom: |r" << playername;
			
				std::stringstream strstream2;
				strstream2 << "|TInterface/ICONS/Achievement_arena_2v2_2:15|t Victoires: " << std::to_string(score.getVictories());

				std::stringstream strstream3;
				strstream3 <<  "|TInterface/ICONS/Achievement_arena_2v2_2:15|t Defaites: " << std::to_string(score.getDefeats());

				std::stringstream strstream4;
				strstream4 << "|TInterface/ICONS/Achievement_arena_2v2_2:15|t Ratio V/D: " << std::to_string(score.getRatio());

				//Affichage du gossip
				player->ADD_GOSSIP_ITEM(2, strstream.str().c_str(), GOSSIP_SENDER_MAIN, 0);
				player->ADD_GOSSIP_ITEM(2, strstream2.str().c_str(), GOSSIP_SENDER_MAIN, 0);
				player->ADD_GOSSIP_ITEM(2, strstream3.str().c_str(), GOSSIP_SENDER_MAIN, 0);
				player->ADD_GOSSIP_ITEM(2, strstream4.str().c_str(), GOSSIP_SENDER_MAIN, 0);
			
			}

			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			return true;

	}
};

void AddSC_npc_classement_bg_all()
{
	new script_npc_classement_bg_all();
}