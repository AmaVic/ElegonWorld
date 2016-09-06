
#include "ScriptMgr.h"
#include <tuple>
#include <vector>
#include <map>
#include <sstream>


enum ITEMS_NOTHING_REQUIRED //AURA_ID
{
	CAT_NR_ID = 0,
	ITEM_BUFF_SPELLPOWER = 696900,
	ITEM_BUFF_ATTACKPOWER = 696904,
	ITEM_BUFF_ATTACKSPEED = 696912,
	ITEM_BUFF_HP = 696908,
	ITEM_BUFF_ARMOR = 696920,
	ITEM_BUFF_RENVOI_SORT = 696916,
	ITEM_BUFF_REGENHP = 696928,
	ITEM_BUFF_MAGIC_RESIST = 696924,

};

enum ITEMS_SECOND_LEVEL
{
	CAT_SL_ID = 1,

	ITEM_BUFF_SPELLPOWER2 = 696901,
	ITEM_BUFF_ATTACKPOWER2 = 696905,
	ITEM_BUFF_ATTACKSPEED2 = 696913,
	ITEM_BUFF_HP2 = 696909,
	ITEM_BUFF_ARMOR2 = 696921,
	ITEM_BUFF_RENVOI_SORT2 = 696917,
	ITEM_BUFF_REGENHP2 = 696928,
	ITEM_BUFF_MAGIC_RESIST2 = 696925,
};

enum ITEMS_THIRD_LEVEL
{
	CAT_TL_ID = 2,

	ITEM_BUFF_SPELLPOWER3 = 696902,
	ITEM_BUFF_ATTACKPOWER3 = 696906,
	ITEM_BUFF_ATTACKSPEED3 = 696914,
	ITEM_BUFF_HP3 = 696910,
	ITEM_BUFF_ARMOR3 = 696922,
	ITEM_BUFF_RENVOI_SORT3 = 696918,
	ITEM_BUFF_REGENHP3 = 696929,
	ITEM_BUFF_MAGIC_RESIST3 = 696926,
};

enum ITEMS_FOURTH_LEVEL
{
	CAT_FL_ID = 3,

	ITEM_BUFF_SPELLPOWER4 = 696903,
	ITEM_BUFF_ATTACKPOWER4 = 696907,
	ITEM_BUFF_ATTACKSPEED4 = 696915,
	ITEM_BUFF_HP4 = 696911,
	ITEM_BUFF_ARMOR4 = 696923,
	ITEM_BUFF_RENVOI_SORT4 = 696919,
	ITEM_BUFF_REGENHP4 = 696930,
	ITEM_BUFF_MAGIC_RESIST4 = 696927,
};

enum ITEM_CATEGORY
{
	CATEGORY_REGULAR,
	CATEGORY_SECOND_LEVEL,
	CATEGORY_THIRD_LEVEL,
	CATEGORY_FOURTH_LEVEL,
};

#define MAX_STACK_PER_AURA 6

#define ITEM_PVP_ON_KILL 20558 //VOIR DANS BattlegroundICW.h

class script_npc_LoL : public CreatureScript
{

public:

	//Constructor:
	script_npc_LoL() : CreatureScript("script_npc_LoL")
	{
		//Remplissage des données: CATEGORIES
		
		//Cat: regulier
		std::vector<uint32> NRRequired = { 0 };
		LoLCat catNR(CATEGORY_REGULAR, "Ameliorations niveau 1", NRRequired);
		//Cat: second niveau
		std::vector<uint32> SLRequired = { ITEM_BUFF_SPELLPOWER, ITEM_BUFF_ATTACKSPEED, ITEM_BUFF_HP, ITEM_BUFF_ARMOR, ITEM_BUFF_RENVOI_SORT, ITEM_BUFF_REGENHP, ITEM_BUFF_MAGIC_RESIST };
		LoLCat catSL(CATEGORY_SECOND_LEVEL, "Ameliorations niveau 2", SLRequired);
		//Cat: troisième niveau
		std::vector<uint32> TLRequired = { ITEM_BUFF_SPELLPOWER2, ITEM_BUFF_ATTACKSPEED2, ITEM_BUFF_HP2, ITEM_BUFF_ARMOR2, ITEM_BUFF_RENVOI_SORT2, ITEM_BUFF_REGENHP2, ITEM_BUFF_MAGIC_RESIST2 };
		LoLCat catTL(CATEGORY_THIRD_LEVEL, "Ameliorations niveau 3", TLRequired);
		//Cat: 4ème niveau
		std::vector<uint32> FLRequired = { ITEM_BUFF_SPELLPOWER3, ITEM_BUFF_ATTACKSPEED3, ITEM_BUFF_HP3, ITEM_BUFF_ARMOR3, ITEM_BUFF_RENVOI_SORT3, ITEM_BUFF_REGENHP3, ITEM_BUFF_MAGIC_RESIST3 };
		LoLCat catFL(CATEGORY_FOURTH_LEVEL, "Ameliorations niveau 4", FLRequired);
		
		//Ajout des catégories:
		mCats.insert(std::make_pair(CAT_NR_ID, catNR));
		mCats.insert(std::make_pair(CAT_SL_ID, catSL));
		mCats.insert(std::make_pair(CAT_TL_ID, catTL));
		mCats.insert(std::make_pair(CAT_FL_ID, catFL));

		



		//Remplissage des données: BUFFS NR
		std::vector<LoLData> dataNR;
		//						AURA_ID					TEXT					   PRICE
		dataNR.push_back(LoLData(ITEM_BUFF_SPELLPOWER,   "Buff degats des sorts",	  30));
		dataNR.push_back(LoLData(ITEM_BUFF_ATTACKSPEED, "Score de Hate", 30));
		dataNR.push_back(LoLData(ITEM_BUFF_ATTACKPOWER, "Puissance d'attaque", 30));
		dataNR.push_back(LoLData(ITEM_BUFF_HP, "Buff HP", 30));
		dataNR.push_back(LoLData(ITEM_BUFF_ARMOR, "Buff Armure", 30));
		dataNR.push_back(LoLData(ITEM_BUFF_RENVOI_SORT, "Buff Renvoi Sort", 30));
		dataNR.push_back(LoLData(ITEM_BUFF_MAGIC_RESIST, "Buff Resilience", 30));


		//On ajoute ces données dans la catégorie appropriée
		mItems.insert(std::make_pair(CAT_NR_ID, dataNR));
		


		//Remplissage des données: BUFFS SL
		std::vector<LoLData> dataSL;
		dataSL.push_back(LoLData(ITEM_BUFF_SPELLPOWER2, "Buff degats des sorts", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_ATTACKPOWER2, "Puissance d'attaque", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_ATTACKSPEED2, "Score de Hate", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_HP2, "Buff HP", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_ARMOR2, "Buff Armure", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_RENVOI_SORT2, "Buff Renvoi Sort", 300));
		dataSL.push_back(LoLData(ITEM_BUFF_MAGIC_RESIST2, "Buff Resilience", 300));

		//On ajoute ces données dans la catégorie appropriée
		mItems.insert(std::make_pair(CATEGORY_SECOND_LEVEL, dataSL));

		std::vector<LoLData> dataTL;
		dataTL.push_back(LoLData(ITEM_BUFF_SPELLPOWER3, "Buff degats des sorts", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_ATTACKPOWER3, "Puissance d'attaque", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_ATTACKSPEED3, "Score de Hate", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_HP3, "Buff HP", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_ARMOR3, "Buff Armure", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_RENVOI_SORT3, "Buff Renvoi Sort", 600));
		dataTL.push_back(LoLData(ITEM_BUFF_MAGIC_RESIST3, "Buff Resilience", 600));

		//On ajoute ces données dans la catégorie appropriée
		mItems.insert(std::make_pair(CATEGORY_THIRD_LEVEL, dataTL));

		std::vector<LoLData> dataFL;
		dataFL.push_back(LoLData(ITEM_BUFF_SPELLPOWER4, "Buff degats des sorts", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_ATTACKPOWER4, "Puissance d'attaque", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_ATTACKSPEED4, "Score de Hate", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_HP4, "Buff HP", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_ARMOR4, "Buff Armure", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_RENVOI_SORT4, "Buff Renvoi Sort", 900));
		dataFL.push_back(LoLData(ITEM_BUFF_MAGIC_RESIST4, "Buff Resilience", 900));

		//On ajoute ces données dans la catégorie appropriée
		mItems.insert(std::make_pair(CATEGORY_FOURTH_LEVEL, dataFL));

	}

public:

	//Override
	bool OnGossipHello(Player* player, Creature* creature) override
	{

		std::function<bool(Player*, std::vector<uint32>)> hasRequirement = [this](Player* p, std::vector<uint32> elems)
		{

			for (uint32 elem : elems)
			{
				if (p->HasAura(elem))
					return true;
			}

			return false;
		};

		for (std::map<uint32, LoLCat>::const_iterator it = mCats.begin(); it != mCats.end(); ++it)
		{
			const LoLCat& cat = it->second;
			if ( (!hasRequirement(player, std::get<2>(cat)) && std::get<0>(cat) != CAT_NR_ID))
				continue;
			player->ADD_GOSSIP_ITEM(0, std::get<1>(cat), GOSSIP_SENDER_MAIN, std::get<0>(cat));
		}

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
	{
		player->PlayerTalkClass->ClearMenus();

		std::vector<LoLData> data;
		switch (action)
		{
		case CAT_NR_ID:
			data = mItems.at(CAT_NR_ID);
			mLastChosenCat[player->GetGUID()] = CAT_NR_ID;
			break;
		case CAT_SL_ID:
			data = mItems.at(CAT_SL_ID);
			mLastChosenCat[player->GetGUID()] = CAT_SL_ID;
			break;
		case CAT_TL_ID:
			data = mItems.at(CAT_TL_ID);
			mLastChosenCat[player->GetGUID()] = CAT_TL_ID;
			break;
		case CAT_FL_ID:
			data = mItems.at(CAT_FL_ID);
			mLastChosenCat[player->GetGUID()] = CAT_FL_ID;
			break;

		default:
			//On récup le prix du buff:
			bool found = false;
			uint32 price = 0;
			std::vector<LoLData> currData = mItems.at(mLastChosenCat.at(player->GetGUID()));
			for (auto it = currData.begin(); (it != currData.end()) && !found; ++it)
			{
				LoLData lfData = *it;
				if (std::get<0>(lfData) == action)
				{
					price = std::get<2>(lfData);
					found = true;
				}
			}

			if (!found)
			{
				player->GetSession()->SendAreaTriggerMessage("ERROR");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}
			else {
				if (player->HasItemCount(ITEM_PVP_ON_KILL, price, true))
				{
					player->DestroyItemCount(ITEM_PVP_ON_KILL, price, true);
					player->CastSpell(player, action, true);
					player->CLOSE_GOSSIP_MENU();
					return true;
				}
				else {
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de MP");
					player->CLOSE_GOSSIP_MENU();
					return true;
				}
			} 
			break;

		}

		//Affichage des propositions
		for (int i = 0; i < data.size(); i++)
		{
			//player->GetSession()->SendAreaTriggerMessage("i = %u", i);;
			LoLData line = data.at(i);
			std::string text = std::get<1>(line) +std::string(" (");
			std::stringstream ss;
			ss << std::get<2>(line);
			text += ss.str();
			text += std::string(") MP");

			player->ADD_GOSSIP_ITEM(0, text, GOSSIP_SENDER_MAIN, std::get<0>(line));

		}
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());

		return true;
	}


private:

	//Rep:
	//N.B: l'aura ID fera office d'actionID
	typedef std::tuple<uint32/*AURAID*/, const char*/*TEXT*/, uint32 /*PRICE*/> LoLData;
	typedef std::tuple<ITEM_CATEGORY, const char*/*TEXT*/, std::vector<uint32/*AURA_REQUIRED*/>> LoLCat;
	std::map<uint32/*CATID*/, LoLCat> mCats;
	std::map <uint32/*CATID*/, std::vector<LoLData>> mItems;
	std::map<uint64, uint32> mLastChosenCat;




};

void AddSC_script_npc_LoL()
{
	new script_npc_LoL;
}