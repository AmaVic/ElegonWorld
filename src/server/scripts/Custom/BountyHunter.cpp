/*
<--------------------------------------------------------------------------->
- Developer(s): Sdyess
- Updated by: Synth
- Complete: %100
- ScriptName: 'BountyHunter'
- Comment: Untested.
- Shared for: Emudevs
<--------------------------------------------------------------------------->
*/
#include "ScriptPCH.h"
#include <cstring>

#define SET_CURRENCY 1  //0 for gold, 1 for honor, 2 for tokens
#define TOKEN_ID 0 // token id

#if SET_CURRENCY == 0
#define BOUNTY_1 "I would like to place a 20g bounty."
#define BOUNTY_2 "I would like to place a 40g bounty."
#define BOUNTY_3 "I would like to place a 100g bounty."
#define BOUNTY_4 "I would like to place a 200g bounty."
#define BOUNTY_5 "I would like to place a 300g bounty."
#define BOUNTY_6 "I would like to place a 400g bounty."
#define BOUNTY_7 "I would like to place a 500g bounty."
#define BOUNTY_8 "I would like to place a 700g bounty."
#endif
#if SET_CURRENCY == 1
#define BOUNTY_1 "J'aimerais placer un avis de recherche de 2000 points d'honneur."
#define BOUNTY_2 "J'aimerais placer un avis de recherche de 4000 points d'honneur."
#define BOUNTY_3 "J'aimerais placer un avis de recherche de 10000 points d'honneur."
#define BOUNTY_4 "J'aimerais placer un avis de recherche de 20000 points d'honneur."
#endif
#if SET_CURRENCY == 2
#define BOUNTY_1 "I would like to place a 1 token bounty."
#define BOUNTY_2 "I would like to place a 3 token bounty."
#define BOUNTY_3 "I would like to place a 5 token bounty."

#endif

#define PLACE_BOUNTY "J'aimerais mettre un avis de recherche sur quelqu'un : "
#define LIST_BOUNTY "Liste des avis de recherches : "
#define NVM "Fermer : "
#define WIPE_BOUNTY "Retirer mon avis recherche : "




#if SET_CURRENCY != 2
//these are just visual prices, if you want to to change the real one, edit the sql further below
enum BountyPrice
{
	BOUNTY_PRICE_1 = 20,
	BOUNTY_PRICE_2 = 40,
	BOUNTY_PRICE_3 = 100,
	BOUNTY_PRICE_4 = 200,
	BOUNTY_PRICE_5 = 300,
	BOUNTY_PRICE_6 = 400,
	BOUNTY_PRICE_7 = 500,
	BOUNTY_PRICE_8 = 700,
};
#else
enum BountyPrice
{
	BOUNTY_PRICE_1 = 1,
	BOUNTY_PRICE_2 = 3,
	BOUNTY_PRICE_3 = 5,
	BOUNTY_PRICE_4 = 10,
};
#endif

bool passChecks(Player * pPlayer, const char * name)
{

	Player * pBounty = sObjectAccessor->FindPlayerByName(name);
	WorldSession * m_session = pPlayer->GetSession();
	if (!pBounty)
	{
		m_session->SendNotification("Ce joueur est hors-ligne ou n'existe pas !");
		return false;
	}
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM bounties WHERE guid ='%u'", pBounty->GetGUID());
	if (result)
	{
		m_session->SendNotification("Il y a deja un avis de recherche sur ce joueur");
		return false;
	}
	if (pPlayer->GetGUID() == pBounty->GetGUID())
	{
		m_session->SendNotification("Vous ne pouvez pas faire cela sur vous meme");
		return false;
	}
	return true;
}

void alertServer(const char * name, int msg)
{
	std::string message;
	if (msg == 1)
	{
		message = "Nouveau avis de recherche sur ";
		message += name;
		message += " tuez-le et recuperer la recompense";
	}
	else if (msg == 2)
	{
		message = "L'avis de recherche sur ";
		message += name;
		message += " est termine";
	}
	sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str(), 0);
}


bool hasCurrency(Player * pPlayer, uint32 required, int currency)
{
	WorldSession *m_session = pPlayer->GetSession();
	switch (currency)
	{
	case 0: //gold
	{
				uint32 currentmoney = pPlayer->GetMoney();
				uint32 requiredmoney = (required * 10000);
				if (currentmoney < requiredmoney)
				{
					m_session->SendNotification("You don't have enough gold!");
					return false;
				}
				pPlayer->SetMoney(currentmoney - requiredmoney);
				break;
	}
	case 1: //honor
	{
				uint32 currenthonor = pPlayer->GetHonorPoints();
				if (currenthonor < required)
				{
					m_session->SendNotification("Vous n'avez pas assez d'honneur");
					return false;
				}
				pPlayer->SetHonorPoints(currenthonor - required);
				break;
	}
	case 2: //tokens
	{
				if (!pPlayer->HasItemCount(TOKEN_ID, required))
				{
					m_session->SendNotification("You don't have enough tokens!");
					return false;
				}
				pPlayer->DestroyItemCount(TOKEN_ID, required, true, false);
				break;
	}

	}
	return true;
}

void flagPlayer(const char * name)
{
	Player * pBounty = sObjectAccessor->FindPlayerByName(name);
	pBounty->SetPvP(true);
	pBounty->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
}

class BountyHunter : public CreatureScript
{
public:
	BountyHunter() : CreatureScript("BountyHunter"){}
	bool OnGossipHello(Player * Player, Creature * Creature)
	{
		Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, PLACE_BOUNTY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, LIST_BOUNTY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, NVM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		if (Player->IsGameMaster())
			Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, WIPE_BOUNTY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

		Player->PlayerTalkClass->SendGossipMenu(907, Creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF + 1:
		{
										   pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, BOUNTY_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5, "", 0, true);
										   pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, BOUNTY_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6, "", 0, true);
										   pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, BOUNTY_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7, "", 0, true);
										   pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_BATTLE, BOUNTY_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8, "", 0, true);
										   pPlayer->PlayerTalkClass->SendGossipMenu(365, pCreature->GetGUID());
										   break;
		}
		case GOSSIP_ACTION_INFO_DEF + 2:
		{
										   QueryResult Bounties = CharacterDatabase.PQuery("SELECT * FROM bounties");

										   if (!Bounties)
										   {
											   pPlayer->PlayerTalkClass->SendCloseGossip();
											   return false;
										   }
#if SET_CURRENCY == 0
										   if (Bounties->GetRowCount() > 1)
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   do
											   {
												   Field * fields = Bounties->Fetch();
												   std::string option;
												   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
												   Field * names = name->Fetch();
												   option = names[0].GetString();
												   option += " ";
												   option += fields[1].GetString();
												   option += " gold";
												   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);
											   } while (Bounties->NextRow());
										   }
										   else
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   Field * fields = Bounties->Fetch();
											   std::string option;
											   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
											   Field * names = name->Fetch();
											   option = names[0].GetString();
											   option += " ";
											   option += fields[1].GetString();
											   option += " gold";
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);

										   }
#endif
#if SET_CURRENCY == 1
										   if (Bounties->GetRowCount() > 1)
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   do
											   {
												   Field * fields = Bounties->Fetch();
												   std::string option;
												   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
												   Field * names = name->Fetch();
												   option = names[0].GetString();
												   option += " ";
												   option += fields[1].GetString();
												   option += " honor";
												   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);
											   } while (Bounties->NextRow());
										   }
										   else
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   Field * fields = Bounties->Fetch();
											   std::string option;
											   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
											   Field * names = name->Fetch();
											   option = names[0].GetString();
											   option += " ";
											   option += fields[1].GetString();
											   option += " honor";
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);

										   }
#endif
#if SET_CURRENCY == 2
										   if (Bounties->GetRowCount() > 1)
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   do
											   {
												   Field * fields = Bounties->Fetch();
												   std::string option;
												   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
												   Field * names = name->Fetch();
												   option = names[0].GetString();
												   option += " ";
												   option += fields[1].GetString();
												   option += " coins";
												   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);
											   } while (Bounties->NextRow());
										   }
										   else
										   {
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Bounties: ", GOSSIP_SENDER_MAIN, 1);
											   Field * fields = Bounties->Fetch();
											   std::string option;
											   QueryResult name = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid='%u'", fields[0].GetUInt64());
											   Field * names = name->Fetch();
											   option = names[0].GetString();
											   option += " ";
											   option += fields[1].GetString();
											   option += " coins";
											   pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, option, GOSSIP_SENDER_MAIN, 1);

										   }
#endif
										   pPlayer->PlayerTalkClass->SendGossipMenu(878, pCreature->GetGUID());
										   break;
		}
		case GOSSIP_ACTION_INFO_DEF + 3:
		{
										   pPlayer->PlayerTalkClass->SendCloseGossip();
										   break;
		}
		case GOSSIP_ACTION_INFO_DEF + 4:
		{
										   CharacterDatabase.PExecute("TRUNCATE TABLE bounties");
										   pPlayer->PlayerTalkClass->SendCloseGossip();
										   break;
		}
		}
		return true;
	}

	bool OnGossipSelectCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char * code)
	{
		pPlayer->PlayerTalkClass->ClearMenus();
		if (uiSender == GOSSIP_SENDER_MAIN)
		{
			if (islower(code[0]))
				toupper(code[0]);

			if (passChecks(pPlayer, code))
			{
				Player * pBounty = sObjectAccessor->FindPlayerByName(code);
				switch (uiAction)
				{
				case GOSSIP_ACTION_INFO_DEF + 5:
				{
												   if (hasCurrency(pPlayer, BOUNTY_PRICE_1, SET_CURRENCY))
												   {
#if SET_CURRENCY != 2
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u','2000', '1')", pBounty->GetGUID());
#else
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u','1', '1')", pBounty->GetGUID());
#endif
													   alertServer(code, 1);
													   flagPlayer(code);
													   pPlayer->PlayerTalkClass->SendCloseGossip();
												   }
												   break;
				}

				case GOSSIP_ACTION_INFO_DEF + 6:
				{
												   if (hasCurrency(pPlayer, BOUNTY_PRICE_2, SET_CURRENCY))
												   {
#if SET_CURRENCY != 2
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '4000', '2')", pBounty->GetGUID());
#else
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '3', '2')", pBounty->GetGUID());
#endif
													   alertServer(code, 1);
													   flagPlayer(code);
													   pPlayer->PlayerTalkClass->SendCloseGossip();
												   }
												   break;
				}
				case GOSSIP_ACTION_INFO_DEF + 7:
				{
												   if (hasCurrency(pPlayer, BOUNTY_PRICE_3, SET_CURRENCY))
												   {
#if SET_CURRENCY != 2
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '10000', '3')", pBounty->GetGUID());
#else
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '5', '3')", pBounty->GetGUID());
#endif
													   alertServer(code, 1);
													   flagPlayer(code);
													   pPlayer->PlayerTalkClass->SendCloseGossip();
												   }
												   break;
				}
				case GOSSIP_ACTION_INFO_DEF + 8:
				{
												   if (hasCurrency(pPlayer, BOUNTY_PRICE_4, SET_CURRENCY))
												   {
#if SET_CURRENCY != 2
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '20000', '4')", pBounty->GetGUID());
#else
													   CharacterDatabase.PExecute("INSERT INTO bounties VALUES('%u', '10', '3')", pBounty->GetGUID());
#endif
													   alertServer(code, 1);
													   flagPlayer(code);
													   pPlayer->PlayerTalkClass->SendCloseGossip();
												   }
												   break;
				}


				}
			}
			else
				pPlayer->PlayerTalkClass->SendCloseGossip();
		}
		return true;
	}
};


class BountyKills : public PlayerScript
{
public:
	BountyKills() : PlayerScript("BountyKills"){}

	void OnPVPKill(Player * Killer, Player * Bounty)
	{
		if (Killer->GetGUID() == Bounty->GetGUID())
			return;

		QueryResult result = CharacterDatabase.PQuery("SELECT * FROM bounties WHERE guid='%u'", Bounty->GetGUID());
		if (!result)
			return;

		Field * fields = result->Fetch();
#if SET_CURRENCY == 0
		switch (fields[2].GetUInt64())
		{
		case 1:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_1 * 10000));
			break;
		case 2:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_2 * 10000));
			break;
		case 3:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_3 * 10000));
			break;
		case 4:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_4 * 10000));
			break;
		case 5:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_5 * 10000));
			break;
		case 6:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_6 * 10000));
			break;
		case 7:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_7 * 10000));
			break;
		case 8:
			Killer->SetMoney(Killer->GetMoney() + (BOUNTY_PRICE_8 * 10000));
			break;
		}
#endif

#if SET_CURRENCY == 1
		switch (fields[2].GetUInt64())
		{
		case 1:
			Killer->SetHonorPoints(Killer->GetHonorPoints() + (2000));
			break;
		case 2:
			Killer->SetHonorPoints(Killer->GetHonorPoints() + (4000));
			break;
		case 3:
			Killer->SetHonorPoints(Killer->GetHonorPoints() + (10000));
			break;
		case 4:
			Killer->SetHonorPoints(Killer->GetHonorPoints() + (20000));
			break;
		}
#endif

#if SET_CURRENCY == 2
		switch (fields[2].GetUInt64())
		{
		case 1:
			Killer->AddItem(TOKEN_ID, BOUNTY_PRICE_1);
			break;
		case 2:
			Killer->AddItem(TOKEN_ID, BOUNTY_PRICE_2);
			break;
		case 3:
			Killer->AddItem(TOKEN_ID, BOUNTY_PRICE_3);
			break;

		}
#endif
		CharacterDatabase.PExecute("DELETE FROM bounties WHERE guid='%u'", Bounty->GetGUID());
		alertServer(Bounty->GetName().c_str(), 2);
	}


};

void AddSC_BountyHunter()
{
	new BountyHunter();
	new BountyKills();
}