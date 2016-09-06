#include "ScriptMgr.h"
#include "Group.h"
#include "RGM.h"
#include "pet.h"
#include "MastoHandler.h"

#define PRICE_CHANGE_RACE 25
#define PRICE_CHANGE_NAME 10
#define PRICE_CHANGE_FACTION 50

uint32 auras[] = { 48162, 48074, 48170, 43223, 36880, 467, 48469 };

class Item_teleporter : public ItemScript
{
public:
	Item_teleporter() : ItemScript("Item_teleporter") { }


	bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/) override
	{
		if (player->GetAreaId() == 3357 || player->InBattleground() || player->InArena() || player->IsInCombat()) {
			player->PlayerTalkClass->ClearMenus();                              // Clears old options
			player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas utiliser votre pierre ici ou vous etes en combat.");
			player->CLOSE_GOSSIP_MENU();
			return true;
		}
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_general:30|t Zone Marchande", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_Arena_2v2_7:30|t Zone FFA", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_Dungeon_Icecrown_IcecrownEntrance:30|t Instances", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/INV_Misc_Gear_01:30|t Utilitaires", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_spy:30|t Votre maison", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_dungeon_gundrak_10man.:30|t Assaut de Zul'gurub", GOSSIP_SENDER_MAIN, 5);
		player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
		return true;

	}
	void OnGossipSelect(Player *player, Item* item, uint32 sender, uint32 uiAction)
	{

		player->PlayerTalkClass->ClearMenus();
		switch (uiAction)
		{
		case 5000: {
					   player->PlayerTalkClass->ClearMenus();
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_general:30|t Zone Marchande", GOSSIP_SENDER_MAIN, 1);
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_Arena_2v2_7:30|t Zone FFA", GOSSIP_SENDER_MAIN, 2);
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_Dungeon_Icecrown_IcecrownEntrance:30|t Instances", GOSSIP_SENDER_MAIN, 4);
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/INV_Misc_Gear_01:30|t Utilitaires", GOSSIP_SENDER_MAIN, 6);
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_spy:30|t Votre maison", GOSSIP_SENDER_MAIN, 7);
					   player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Achievement_dungeon_gundrak_10man.:30|t Assaut de Zul'gurub", GOSSIP_SENDER_MAIN, 5);
					   player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
		}
			break;
		case 1: // Zone Marchande
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/ability_paladin_shieldofthetemplar:20|t Equipements JcE", GOSSIP_SENDER_MAIN, 200);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_banner_03:20|t Equipements JcJ", GOSSIP_SENDER_MAIN, 201);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_shirt_16:20|t Transmogrifications", GOSSIP_SENDER_MAIN, 202);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/INV_Elegon_World:20|t Boutique", GOSSIP_SENDER_MAIN, 203);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_glyph_minorhunter:20|t Glyphes", GOSSIP_SENDER_MAIN, 205);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_misc_note_01:20|t Metiers", GOSSIP_SENDER_MAIN, 206);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_misc_herb_04:20|t Espace vert", GOSSIP_SENDER_MAIN, 207);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/achievement_guildperk_hastyhearth:20|t Auberge", GOSSIP_SENDER_MAIN, 208);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 5000);
			player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
			break;


		case 200:
			player->TeleportTo(1, -3714.115234f, -4540.350098f, 10.411351f, 3.668732f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case 201:
			player->TeleportTo(1, -3751.985596f, -4338.085449f, 10.238964f, 1.620422f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 202:
			player->TeleportTo(1, -3746.809570f, -4439.582520f, 30.568769f, 3.862734f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 203:
			player->TeleportTo(1, -3837.564206f, -4452.482910f, 11.337552f, 0.435244f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 204:
			player->TeleportTo(1, -3693.937012f, -4555.739258f, 11.910572f, 0.435244f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 205:
			player->TeleportTo(1, -3706.787354f, -4545.706543f, 25.833937f, 0.448092f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 206:
			player->TeleportTo(1, -3825.409180f, -4537.025391f, 9.218354f, 3.931294f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 207:
			player->TeleportTo(1, -3800.267334f, -4363.841309f, 14.150029f, 1.576419f);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 208:
			player->TeleportTo(1, -3624.044434f, -4448.898438f, 14.247352f, 4.684493f);
			player->CLOSE_GOSSIP_MENU();

			break;

		case 2: // Zone FFA
			if (sMastoHandler->eventIsStarted()) {
				player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas vous teleporter pendant l'event mastodonte");
				player->CLOSE_GOSSIP_MENU();

			}
			else {
				switch (urand(0, 4)) {
				case 0:
					player->TeleportTo(1, -1681.537476f, -4399.880859f, 20.098492f, 1.538679f);
					break;
				case 1:
					player->TeleportTo(1, -1647.343262f, -4287.811035f, 16.951925f, 4.150134f);
					break;
				case 2:
					player->TeleportTo(1, -1601.527344f, -4376.117676f, 4.252836f, 2.93277f);
					break;
				case 3:
					player->TeleportTo(1, -1732.508789f, -4380.767578f, 11.251544f, 0.643337f);
					break;
				case 4:
					player->TeleportTo(1, -1672.381714f, -4380.767578f, 11.251544f, 0.643337f);
					break;
				}

			}
			break;

		case 3: // Zone Duel

			break;

		case 4: // Instance
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_dungeon_ulduarraid_misc_02:30|t Donjon : Uldaman", GOSSIP_SENDER_MAIN, 10);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_dungeon_ulduarraid_archway_01:30|t Donjon : Rage-feu", GOSSIP_SENDER_MAIN, 11);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_dungeon_ulduarraid_misc_05:30|t Donjon : Gnomeregan", GOSSIP_SENDER_MAIN, 12);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_misc_head_murloc_01:30|t Donjon : Ile des murlocs", GOSSIP_SENDER_MAIN, 14);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/inv_helmet_52:30|t Donjon : Donjon Ecarlate", GOSSIP_SENDER_MAIN, 15);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "|TInterface/ICONS/Achievement_dungeon_gundrak_heroic.:30|t Raid : Zul'Gurub", GOSSIP_SENDER_MAIN, 13);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 5000);

			player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
			break;

		case 5: // Instance
			player->TeleportTo(0, -12018.661133f, -536.563538f, 12.086570f, 0.085931f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case 6:
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_01:30|t Section : Compte", GOSSIP_SENDER_MAIN, 600);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_03:30|t Section : Personnage", GOSSIP_SENDER_MAIN, 601);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_05:30|t Section : Campagnons", GOSSIP_SENDER_MAIN, 7000);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 5000);

			player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
			break;

		case 7:
			if (player->GetGroup())
			{
				player->GetSession()->SendAreaTriggerMessage("Vous ne pouvez vous teleporter laba en groupe.");
				player->CLOSE_GOSSIP_MENU();
			}
			else {
				player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_NORMAL);
				player->TeleportTo(560, 2666.530518f, 657.063110f, 57.64389f, 2.663806f);
			}
			break;
		case 10:
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_HEROIC )
			{
				if (!player->HasItemCount(100050, 1) || player->isVIP())
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->TeleportTo(70, -227.184998f, 55.99054f, -46.037479f, 1.502312f);

				}
			}
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
			{
				player->TeleportTo(70, -227.184998f, 55.99054f, -46.037479f, 1.502312f);

			}
			break;

		case 11:
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_HEROIC)
			{
				if (!player->HasItemCount(100050, 1) || player->isVIP())
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->TeleportTo(389, 3.270822f, -14.630294f, -17.742393f, 4.447617f);

				}
			}
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
			{
				player->TeleportTo(389, 3.270822f, -14.630294f, -17.742393f, 4.447617f);

			}

			break;

		case 12:
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_HEROIC)
			{
				if (!player->HasItemCount(100050, 1) || player->isVIP())
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->TeleportTo(90, -338.950104f, 1.201784f, -151.502533f, 2.729157f);

				}
			}
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
			{
				player->TeleportTo(90, -338.950104f, 1.201784f, -151.502533f, 2.729157f);

			}

			break;

		case 13:
			player->GetSession()->SendAreaTriggerMessage("Le Raid Zul'Gurub est bientot disponible !");
			player->CLOSE_GOSSIP_MENU();
			/*player->TeleportTo(309, -11916.900391f, -1248.359985f, 92.533401f, 4.724170f);*/
			break;

		case 14:
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_HEROIC)
			{
				if (!player->HasItemCount(100050, 1) || player->isVIP())
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->TeleportTo(585, 251.204086f, -391.288300f, -88.186325f, 5.125843f);

				}
			}
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
			{
				player->TeleportTo(585, 251.204086f, -391.288300f, -88.186325f, 5.125843f);
			}
			break;

		case 15:
			player->SetDungeonDifficulty(DUNGEON_DIFFICULTY_HEROIC);
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_HEROIC)
			{
				if (!player->HasItemCount(100050, 1))
				{
					player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas l'acces requis pour ce donjon heroique.");
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{
					player->TeleportTo(189, 853.067444f, 1318.125000f, 18.672319f, 1.513635f);

				}
			}
			if (player->GetDungeonDifficulty() == DUNGEON_DIFFICULTY_NORMAL)
			{
				player->TeleportTo(189, 853.067444f, 1318.125000f, 18.672319f, 1.513635f);

			}
			break;


		case 600: {
					  player->PlayerTalkClass->ClearMenus();
					  QueryResult result_level = LoginDatabase.PQuery("SELECT points FROM account WHERE id = %u", player->GetSession()->GetAccountId());
					  Field *fields = result_level->Fetch();
					  uint32 points = fields[0].GetUInt32();
					  std::stringstream info_house;
					  info_house << "|TInterface/ICONS/INV_Elegon_World:30|t Transformer vos : " << points << " point(s) Elegon World en marques Elegon World" << std::endl;
					  player->ADD_GOSSIP_ITEM(0, info_house.str(), GOSSIP_SENDER_MAIN, 700);
					  player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());


		}
			break;
		case 700: {			QueryResult result_level = LoginDatabase.PQuery("SELECT points FROM account WHERE id = %u", player->GetSession()->GetAccountId());
			Field *fields = result_level->Fetch();
			uint32 points = fields[0].GetUInt32();
			
			if (points == 0)
			{
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas de points Elegon World");

			}
			if (player->isVIP())
			{
				LoginDatabase.PQuery("UPDATE account SET points = points - %u WHERE id = %u", points, player->GetSession()->GetAccountId());
				uint32 vip_multiplier = points * 2;
				player->AddItem(456500, vip_multiplier);
				player->CLOSE_GOSSIP_MENU();

			}
			else {
				LoginDatabase.PQuery("UPDATE account SET points = points - %u WHERE id = %u", points, player->GetSession()->GetAccountId());
				player->AddItem(456500, points);
				player->CLOSE_GOSSIP_MENU();
			}
			
		}

			break;
		case 601:
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/spell_shadow_auraofdarkness:35|t |cffFF0000Mode Defi|r", GOSSIP_SENDER_MAIN, 8007);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_01:30|t Tous les buffs", GOSSIP_SENDER_MAIN, 8001);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_01:30|t Reinitialiser points de talents", GOSSIP_SENDER_MAIN, 8002);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_01:30|t Double specialisation", GOSSIP_SENDER_MAIN, 8010);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_faction_lorewalkers:30|t Changement de race (25 marques Elegon World)", GOSSIP_SENDER_MAIN, 8004);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_faction_lorewalkers:30|t Changement de faction (50 marques Elegon World)", GOSSIP_SENDER_MAIN, 8005);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_faction_lorewalkers:30|t Changement de nom (10 marques Elegon World)", GOSSIP_SENDER_MAIN, 8006);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 5000);

			player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
			break;
		case 7000:
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_holy_ardentdefender:30|t Compagnon Tank (30 minutes)(1 marque Elegon World)", GOSSIP_SENDER_MAIN, 6000);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_holy_blessingofstrength:30|t Compagnon Heal (30 minutes)(1 marque Elegon World)", GOSSIP_SENDER_MAIN, 6001);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_holy_aspiration:30|t Compagnon DPS (30 minutes)(1 marque Elegon World)", GOSSIP_SENDER_MAIN, 6002);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Retour", GOSSIP_SENDER_MAIN, 5000);
			player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
			break;
		case 6000: {
					   if (player->HasItemCount(456500, 1, true)) {
						   Pet * creature = player->SummonPet(361220, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), PetType::SUMMON_PET, 1800000);
						   player->CLOSE_GOSSIP_MENU();
						   player->DestroyItemCount(456500, 1, true);
					   }
					   else {
						   player->CLOSE_GOSSIP_MENU();
						   player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

					   }
		}

			break;
		case 6001: {
					   if (player->HasItemCount(456500, 1, true)) {
						   Pet * creature = player->SummonPet(361221, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), PetType::SUMMON_PET, 1800000);
						   player->CLOSE_GOSSIP_MENU();
						   player->DestroyItemCount(456500, 1, true);
					   }
					   else {
						   player->CLOSE_GOSSIP_MENU();
						   player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

					   }
		}

			break;
		case 6002: {
					   if (player->HasItemCount(456500, 1, true)) {
						   Pet * creature = player->SummonPet(361223, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), PetType::SUMMON_PET, 1800000);
						   player->CLOSE_GOSSIP_MENU();
						   player->DestroyItemCount(456500, 1, true);
					   }
					   else {
						   player->CLOSE_GOSSIP_MENU();
						   player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

					   }
		}


			break;


		case 8000: {
					   player->PlayerTalkClass->ClearMenus();
					   Creature * creature = player->SummonCreature(1000005, player->GetPosition(), TEMPSUMMON_TIMED_DESPAWN, 30000);
					   player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());


		}
			break;
		case 8001:
			for (int i = 0; i < 7; i++)
				player->AddAura(auras[i], player);
			player->CLOSE_GOSSIP_MENU();

			break;
		case 8002:
			player->resetTalents(true);
			player->SendTalentsInfoData(true);
			player->SendInitialPacketsBeforeAddToMap();
			player->CLOSE_GOSSIP_MENU();
			break;
		case 8003:
			if (player->HasItemCount(456500, 1, true)) {
				player->DestroyItemCount(456500, 1, true);
				player->CLOSE_GOSSIP_MENU();
			}
			else {
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

			}

			break;
		case 8004:
			if (player->HasItemCount(456500, PRICE_CHANGE_RACE, true)) {
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
				player->CLOSE_GOSSIP_MENU();
				player->DestroyItemCount(456500, PRICE_CHANGE_RACE, true);
				player->GetSession()->SendAreaTriggerMessage("A votre prochaine connexion, vous pourrez changer de race");
			}
			else {
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

			}
			break;

		case 8005:
			if (player->HasItemCount(456500, PRICE_CHANGE_FACTION, true)) {
				player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
				player->CLOSE_GOSSIP_MENU();
				player->DestroyItemCount(456500, PRICE_CHANGE_FACTION, true);
			}
			else {
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");

			}
			break;
		case 8006:
			if (player->HasItemCount(456500, PRICE_CHANGE_NAME, true)) {
				player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
				player->CLOSE_GOSSIP_MENU();
				player->DestroyItemCount(456500, PRICE_CHANGE_NAME, true);
			}
			else {
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendAreaTriggerMessage("Vous n'avez pas assez de marques Elegon World");
			}
			break;
		case 8007:
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "J'accepte", GOSSIP_SENDER_MAIN, 8008);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Non merci", GOSSIP_SENDER_MAIN, 8009);
			player->PlayerTalkClass->SendGossipMenu(45210, item->GetGUID());
			break;
		case 8008:
			player->AddAura(120145, player);
			player->CLOSE_GOSSIP_MENU();
			break;
		case 8009:
			player->CLOSE_GOSSIP_MENU();
			break;
		case 8010:
			player->CastSpell(player, 63624);
			player->CLOSE_GOSSIP_MENU();
			break;
		}
	}


};

void AddSC_item_script_teleport()
{
	new Item_teleporter();
}