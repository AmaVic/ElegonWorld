#include "ScriptMgr.h"
#define ITEM_ID 1000

class Npc_Blood : public CreatureScript
{

public:
	Npc_Blood() : CreatureScript("Npc_Blood") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
		if (!player->HasItemCount(443150, 1, true)) { player->GetSession()->SendAreaTriggerMessage("Vous ne participez pas au tournoi de l'ile de paques"); return false; }
		if (player->IsInCombat()) { player->GetSession()->SendAreaTriggerMessage("Vous etes en combat !");return false;}
		QueryResult result_level = CharacterDatabase.PQuery("SELECT vh_blood FROM characters WHERE guid = %u", player->GetGUID());
		Field *fields = result_level->Fetch();
		uint32 vh_blood = fields[0].GetUInt32();
		std::stringstream info_house;
		info_house << "|TInterface/ICONS/Ability_backstab:30|t Vous avez fait : " << vh_blood << " VH en mode buveur de sang" << std::endl;
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, info_house.str() , GOSSIP_SENDER_MAIN, 10000);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/spell_deathknight_bloodboil:30|t Passer en mode : |cffFF0000Buveur de sang|r", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_seal:30|t Passer en mode : |cff3ADF00Normal|r", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Fermer", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(25005, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
				player->GetSession()->SendAreaTriggerMessage("Vous etes maintenant en mode buveur de sang (mode JcJ): tuer des joueurs vous rapporte des points sur l'ile de paques. Attention, les autres joueurs peuvent vous attaquer aussi !");
				player->AddAura(12898, player);
				player->SetPvP(true);
				player->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
				player->CLOSE_GOSSIP_MENU();
			break;

			case 2:
				player->GetSession()->SendAreaTriggerMessage("Vous etes sortis du mode buveur de sang");
				player->RemoveAllAuras();
				player->SetPvP(false);
				player->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
				player->CLOSE_GOSSIP_MENU();
				break;
			case 3:
				player->CLOSE_GOSSIP_MENU();
				break;
			}
		}
		return true;
	}

};

class Npc_hunter : public CreatureScript
{

public:
	Npc_hunter() : CreatureScript("Npc_hunter") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{
		if (!player->HasItemCount(443150, 1, true)) { player->GetSession()->SendAreaTriggerMessage("Vous ne participez pas au tournoi de l'ile de paques"); return false; }		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_chargepositive:30|t Echanger mes ressources en points", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Classement et vos points", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Fermer", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(25006, creature->GetGUID());
		return true;
	}
	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{

		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			case 1:
			{				uint32 item_count = player->GetItemCount(502261, true);
			player->GetSession()->SendAreaTriggerMessage("Vous recevez %u points pour le tournoi de l'ile de paques", item_count);
			CharacterDatabase.PQuery("UPDATE characters SET war_points = war_points + %u WHERE guid = %u", item_count, player->GetGUID());
			player->DestroyItemCount(502261, item_count, true);
			player->CLOSE_GOSSIP_MENU(); }

				break;

			case 3:
				player->CLOSE_GOSSIP_MENU();
				break;
			case 4: {

						QueryResult result = CharacterDatabase.PQuery("SELECT name, war_points FROM characters ORDER BY war_points DESC");
						player->CLOSE_GOSSIP_MENU();
						do {

							Field *fields = result->Fetch();
							uint32 war_points = fields[1].GetUInt32();
							std::string  name = fields[0].GetString();
							char msg[250];
							snprintf(msg, 250, "|cffDBA901Nom du joueur : |r |cffB40431%s|r . |cff0174DFpoints :|r |cffDF01A5%u|r", name, war_points);
							player->MonsterWhisper(msg, player);

						} while (result->NextRow());

						player->PlayerTalkClass->ClearMenus();

			}

				break;
			}
		}
		return true;
	}


};
class item_hearth_Vemriss : public ItemScript
{
public:
	item_hearth_Vemriss() : ItemScript("item_hearth_Vemriss") { }

	bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/) override
	{
		player->DestroyItemCount(102830, 1, true);
		CharacterDatabase.PQuery("UPDATE characters SET war_points = war_points + 1000 WHERE guid = %u", player->GetGUID());
		player->GetSession()->SendAreaTriggerMessage("Vous venez de gagner 1000 points pour le tournoi de l'ile de paques");
		return false;
	}
};

class On_player_die_zone : public PlayerScript
{
public:
	On_player_die_zone() : PlayerScript("On_player_die_zone") {}

	void OnPVPKill(Player* killer, Player* killed) { 

		QueryResult result_level = CharacterDatabase.PQuery("SELECT vh_blood FROM characters WHERE guid = %u", killer->GetGUID());

		if (!result_level)
			return;

		Field *fields = result_level->Fetch();
		uint32 vh_blood = fields[0].GetUInt32();

		if (killer->GetAreaId() != 3357)
			return;

		if (killed == killer)
			return;

		if (vh_blood == 50) {

			CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(184);
			killer->SetTitle(titleInfo);
		}
		if (vh_blood == 100) {

			CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(183);
			killer->SetTitle(titleInfo);
		}
		if (vh_blood == 200) {

			CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(185);
			killer->SetTitle(titleInfo);
		}


	
		if (killer->HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP)){

			killer->GetSession()->SendAreaTriggerMessage("Vous avez tue un joueur en mode buveur de sang, vous gagnez 25 points pour le tournoi de l'ile de paques");
			CharacterDatabase.PQuery("UPDATE characters SET war_points = war_points + 25 WHERE guid = %u", killer->GetGUID());
			CharacterDatabase.PQuery("UPDATE characters SET vh_blood = vh_blood + 1 WHERE guid = %u", killer->GetGUID());
			killer->SummonGameObject(194479, killer->GetPositionX(), killer->GetPositionY(), killer->GetPositionZ(), killer->GetOrientation(), 0, 0, 0, 0, 30000);

		}
	

		killed->ResurrectPlayer(100, false);
		killed->TeleportTo(0, -11780.299805f, 1223.007813f, 8.062670f, 1.957499f);
		killed->GetSession()->SendAreaTriggerMessage("Vous avez ete tue par un buveur de sang !");

	}

};
class Top_NPC : public CreatureScript
{

public:
	Top_NPC() : CreatureScript("Top_NPC") {}

	bool OnGossipHello(Player *player, Creature *creature)
	{

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_pvp_h_08:35|t Classement", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Fermer", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(25007, creature->GetGUID());
		return true;
	}



	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (uiAction)
			{
			
			case 3: {
						creature->MonsterWhisper("Aurevoir !", player);
						player->CLOSE_GOSSIP_MENU();

			}


				break;
			}
		}
		return true;
	}


};

class Onloginwar : public PlayerScript
{
public:
	Onloginwar() : PlayerScript("Onloginwar") {}

	void OnLogin(Player* player, bool firstLogin) {

		if (firstLogin)
			return; 

		CharacterDatabase.PQuery("UPDATE characters SET date_day = CURRENT_DATE WHERE guid = %u", player->GetGUID());
		QueryResult result = CharacterDatabase.PQuery("SELECT DAY(date_end) AS jour, DAY(date_day) AS jour, war_points FROM characters WHERE guid = %u", player->GetGUID()); 

			if (!result)
				return;


			Field *fields = result->Fetch();
			uint32 date_end = fields[0].GetUInt32();
			uint32 date_day = fields[1].GetUInt32(); 
			uint32 war_points = fields[2].GetUInt32();
			uint32 item_add = war_points / 100;

			if (date_end == date_day) {

				player->AddItem(ITEM_ID, item_add);
				player->GetSession()->SendNotification("Vous avez gagne un total de %u marques Elegon World cette semaine. Bonne chance pour la suite.", item_add);
				CharacterDatabase.PQuery("UPDATE characters SET war_points = 0 WHERE guid = %u", player->GetGUID());
				CharacterDatabase.PQuery("UPDATE characters SET date_data = CURRENT_DATE WHERE guid = %u", player->GetGUID());
				CharacterDatabase.PQuery("UPDATE characters SET date_end = CURRENT_DATE + INTERVAL 5  DAY WHERE guid = %u", player->GetGUID());
				CharacterDatabase.PQuery("UPDATE characters SET date_day = CURRENT_DATE WHERE guid = %u", player->GetGUID());

			}
	}
};
void AddSC_ScriptWarSytem()
{
	new Npc_Blood();
	new Npc_hunter();
	new item_hearth_Vemriss();
	new On_player_die_zone();
	new Top_NPC();
	new Onloginwar;
}