
#include "VHScore.h"

//---------------------------------------------
//Constructeur
//---------------------------------------------
VHScore::VHScore() : BaseScore<KillsScore>()
{

}


//---------------------------------------------
//Fonctions membres publiques à overrider
//---------------------------------------------
void VHScore::generateRaking()
{
	QueryResult result = CharacterDatabase.PQuery("SELECT name, totalKills FROM characters ORDER BY totalKills DESC");
	do
	{
		Field* fields = result->Fetch();

		std::string playerName = fields[0].GetString();
		uint32 kills = fields[1].GetUInt32();

		KillsScore score(playerName, kills);
		mRanking.push_back(score);

	} while (result->NextRow());
}

std::vector<std::string> VHScore::getRankingStringList()
{
	std::vector<std::string> vectStr;
	for (auto elem : mRanking)
	{
		std::string currStr = elem.mPlayerName + ": " + std::to_string(elem.kills) + " VH";
		vectStr.push_back(currStr);
	}

	return vectStr;
}