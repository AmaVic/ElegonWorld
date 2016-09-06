#include "ScriptMgr.h"
#include "SponsorshipHandler.h"
#include "Group.h"

#define SPONSORSHIP_AURA_HEAL_DMG_HP  722220
#define SPONSORSHIP_AURA_HONOR 722221

class script_group_sponsorship : public GroupScript
{

public: //Construct

	script_group_sponsorship() : GroupScript("script_group_sponsorship"){}

public:

	//Override:
	void OnAddMember(Group* group, uint64 guid)
	{
		Player* playerAdded = sObjectAccessor->FindPlayer(guid);
		if (!playerAdded)
			return;

		Player* secondPlayer;
		//on parcoure tous les membres du groupe sauf le nouveau player pour voir si quelqu'un est en parrainage avec le joueur ajouté
		for (auto it = group->GetMemberSlots().begin(); it != group->GetMemberSlots().end(); ++it)
		{
			if (it->guid == guid)
				continue;

			secondPlayer = sObjectAccessor->FindPlayer(it->guid);
			if (!secondPlayer)
				continue;

			if (SponsorshipHandler::areInSponsorship(playerAdded->GetSession()->GetAccountId(), secondPlayer->GetSession()->GetAccountId()))
			{
				//On ajoute les buffs
				playerAdded->AddAura(SPONSORSHIP_AURA_HEAL_DMG_HP, playerAdded);
				playerAdded->AddAura(SPONSORSHIP_AURA_HONOR, playerAdded);

				secondPlayer->AddAura(SPONSORSHIP_AURA_HEAL_DMG_HP, secondPlayer);
				secondPlayer->AddAura(SPONSORSHIP_AURA_HONOR, secondPlayer);
			}
		}
	
	}

	void OnRemoveMember(Group* /*group*/, uint64 guid, RemoveMethod /*method*/, uint64 /*kicker*/, const char* /*reason*/)
	{
		Player* p = sObjectAccessor->FindPlayer(guid);
		if (!p)
			return;

		if (p->HasAura(SPONSORSHIP_AURA_HEAL_DMG_HP))
			p->RemoveAura(SPONSORSHIP_AURA_HEAL_DMG_HP);
		if (p->HasAura(SPONSORSHIP_AURA_HONOR))
			p->RemoveAura(SPONSORSHIP_AURA_HONOR);
	}

	void OnDisband(Group* group)
	{
		for (auto it = group->GetMemberSlots().begin(); it != group->GetMemberSlots().end(); ++it)
		{
			Player* p = sObjectAccessor->FindPlayer(it->guid);
			if (!p)
				continue;

			if (p->HasAura(SPONSORSHIP_AURA_HEAL_DMG_HP))
				p->RemoveAura(SPONSORSHIP_AURA_HEAL_DMG_HP);
			if (p->HasAura(SPONSORSHIP_AURA_HONOR))
				p->RemoveAura(SPONSORSHIP_AURA_HONOR);
		}
	}



};

class script_remove_sponsorship : public PlayerScript
{

public:

	script_remove_sponsorship() : PlayerScript("script_remove_sponsorship"){}

	void OnLogin(Player* player, bool firstLogin)
	{
		SponsorshipHandler::removeSponsorshipIfExpired();
	}

	

};

class script_command_sponsorship : public CommandScript
{

public:

	script_command_sponsorship() : CommandScript("script_command_sponsorship"){}

	ChatCommand* GetCommands() const
	{
		static ChatCommand sponsorTable[] =
		{
			{ "tele", rbac::RBAC_PERM_COMMAND_SPONSORSHIP_TELE, true, &HandleSponsorTeleCommand, "", NULL },
			{ "infos", rbac::RBAC_PERM_COMMAND_SPONSORSHIP_INFOS, true, &HandleSponsorInfosCommand, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};

		static ChatCommand commandTable[] =
		{
			{ "parrainage", rbac::RBAC_PERM_COMMAND_SPONSORSHIP, true, NULL, "", sponsorTable },
			{ NULL, 0, false, NULL, "", NULL }
		};

		return commandTable;
	}


	static bool HandleSponsorTeleCommand(ChatHandler* handler, char const* args)
	{
		if (handler->GetSession()->GetPlayer()->IsInCombat())
		{
			handler->GetSession()->SendAreaTriggerMessage("Vous ne pouvez pas faire ceci en combat...");
			return true;
		}

		uint64 friendGUID = 0;
		bool sponsorshipFriendOnline = SponsorshipHandler::findOnlineSponsorOrSponsored(handler->GetSession()->GetAccountId(), handler->GetSession()->GetPlayer()->GetGUID(), friendGUID);
		if (sponsorshipFriendOnline)
		{
			//Alors on téléporte le joueur sur son ami
			Player* friendPlayer = sObjectAccessor->FindPlayer(friendGUID);
			if (!friendPlayer)
			{
				handler->GetSession()->SendAreaTriggerMessage("Une erreur est survenue...");
				return true;
			}

			if (!friendPlayer->IsInWorld())
			{
				handler->GetSession()->SendAreaTriggerMessage("%s n'est pas en ligne", friendPlayer->GetName().c_str());
				return true;
			}

			Map* map = friendPlayer->GetMap();
			if (map->IsBattlegroundOrArena() || map->IsDungeon() || map->IsRaid())
			{
				handler->GetSession()->SendAreaTriggerMessage("Impossible de se teleporter, votre ami est soit dans un champ de bataille, soit dans une arene, soit dans une instance ou un raid");
				return true;
			}

			//on TP si tout est ok:
			handler->GetSession()->GetPlayer()->TeleportTo(friendPlayer->GetWorldLocation());
			return true;
		}

		return true;
	}

	static bool HandleSponsorInfosCommand(ChatHandler* handler, char const* args)
	{
		if (!SponsorshipHandler::isInSponsorship(handler->GetSession()->GetAccountId()))
		{
			handler->GetSession()->SendAreaTriggerMessage("Vous n'etes dans aucune relation de parrainage");
			return true;
		}

		handler->GetSession()->SendAreaTriggerMessage("Il vous reste %u jours de parrainage", SponsorshipHandler::timeLeft(handler->GetSession()->GetAccountId()));
		return true;
	}


};


void AddSC_scripts_sponsorship()
{
	new script_group_sponsorship();
	new script_remove_sponsorship();
	new script_command_sponsorship();
}