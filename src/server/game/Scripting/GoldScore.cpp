
#include "GoldScore.h"


//---------------------------------------------
//Constructeur
//---------------------------------------------
GoldScore::GoldScore() : BaseScore<GScore>()
{

}



//---------------------------------------------
//Fonctions membres publiques à overrider
//---------------------------------------------
void GoldScore::generateRaking()
{
	QueryResult result = CharacterDatabase.PQuery("SELECT name, money FROM characters ORDER BY money DESC");
	do
	{
		Field* field = result->Fetch();
		std::string name = field[0].GetString();
		uint64      money = field[1].GetUInt64();

		GScore goldScore(name, money);
		mRanking.push_back(goldScore);

	} while (result->NextRow());
}

std::vector<std::string> GoldScore::getRankingStringList()
{
	std::vector<std::string> vectToReturn;
	for (auto elem : mRanking)
	{
		std::string currStr = elem.mPlayerName + ": " + std::to_string(elem.mMoney) + " PO";
		vectToReturn.push_back(currStr);
	}

	return vectToReturn;
}

void GoldScore::updateScore(uint64 playerGUID, uint32 amount)
{
	//On créé la ligne du joueur si elle n'existe pas
	BaseScore::updateScore(playerGUID, amount);

	//On met à jour la ligne
	CharacterDatabase.PExecute("UPDATE rankings SET gold=gold+%u WHERE playerGUID=%u", amount, playerGUID);
}