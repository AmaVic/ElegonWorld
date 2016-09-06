

#include "ScriptMgr.h"
#include "Group.h"
#include <vector>
#include <algorithm>

#define QUEST_ID 200211


class script_grade_pve : public PlayerScript
{
public:
	
	script_grade_pve() : PlayerScript("script_grade_pve")
	{
		mCreaturesToCount.push_back(3);
	}

	// Called when a player kills a creature
	void OnCreatureKill(Player* killer, Creature* killed)
	{
		std::vector<uint32>::iterator it = std::find_if(mCreaturesToCount.begin(), mCreaturesToCount.end(), [killed](uint32 const& item){
			return killed->GetEntry() == item;
		});

		if (it == mCreaturesToCount.end())
			return;

		if (!killer->GetGroup())
		{

			uint32 currentKillCount = 0;

			QueryResult check = CharacterDatabase.PQuery("SELECT * FROM grades_pve WHERE playerGUID=%u AND creatureId=%u", killer->GetGUID(), killed->GetEntry());
			if (!check)
			{
				return;
			}

			if (check)
			{
				Field* fields = check->Fetch();
				currentKillCount = fields[2].GetUInt32() + 1;

				//On met à jour
				CharacterDatabase.PQuery("UPDATE grades_pve SET count=count+1 WHERE playerGUID=%u AND creatureId=%u", killer->GetGUID(), killed->GetEntry());
			}
			else {
				CharacterDatabase.PQuery("INSERT INTO grades_pve VALUES (%u, %u, 1)", killed->GetEntry(), killer->GetGUID());
				currentKillCount = 1;
			}

			//GRADES: Tous les 10 kills
			Quest const* quest = sObjectMgr->GetQuestTemplate(QUEST_ID);
			Object * giver = killer->ToCreature();
			if ((currentKillCount % 10 == 0) && currentKillCount <= 100)
			{
				killer->GetSession()->SendAreaTriggerMessage("Bravo, vous avez atteint l'objectif ! Jetez un oeil à vos quetes... Une récompense vous attend !");
				killer->AddQuest(quest, giver);
			}
		}
		else {

			Group* group = killer->GetGroup();
			for (auto it = group->GetMemberSlots().begin(); it != group->GetMemberSlots().end(); it++)
			{
				uint32 playerGUID = it->guid;
				Player* player = sObjectAccessor->FindPlayer(playerGUID);
				if (!player)
					continue;
				uint32 currentKillCount = 0;

				QueryResult check = CharacterDatabase.PQuery("SELECT * FROM grades_pve WHERE playerGUID=%u AND creatureId=%u", player->GetGUID(), killed->GetEntry());
				if (!check)
				{
					return;
				}
				if (check)
				{
					Field* fields = check->Fetch();
					currentKillCount = fields[2].GetUInt32() + 1;

					//On met à jour
					CharacterDatabase.PQuery("UPDATE grades_pve SET count=count+1 WHERE playerGUID=%u AND creatureId=%u", player->GetGUID(), killed->GetEntry());
				}
				else {
					CharacterDatabase.PQuery("INSERT INTO grades_pve VALUES (%u, %u, 1)", killed->GetEntry(), player->GetGUID());
					currentKillCount = 1;
				}

				//GRADES: Tous les 10 kills
				Quest const* quest = sObjectMgr->GetQuestTemplate(QUEST_ID);
				Object * giver = player->ToCreature();
				if ((currentKillCount % 10 == 0) && currentKillCount <= 100)
				{
					player->GetSession()->SendAreaTriggerMessage("Bravo, vous avez atteint l'objectif ! Jetez un oeil vos quetes... Une récompense vous attend !");
					player->AddQuest(quest, giver);
				}

			}

		}
	}
	
private:

	std::vector<uint32> mCreaturesToCount;
};

void AddSC_grades_pve()
{
	new script_grade_pve();
}