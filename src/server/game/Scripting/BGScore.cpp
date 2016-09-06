

#include "BGScore.h"

//---------------------------------------
//Fonctions publiques statiques
//---------------------------------------
std::vector<BGScore> BGScore::getBGScores(uint32 bg_id)
{
	std::vector<BGScore> scores;
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM bgscores WHERE bgID=%u ORDER BY victories DESC", bg_id);
	if (!result)
		return scores;

	do
	{
		Field* fields = result->Fetch();

		uint32 playerGUID = fields[0].GetUInt32();
		BGScore score(playerGUID, bg_id);
		scores.push_back(score);
	} while (result->NextRow());

	return scores;
}

std::vector<uint32 /*BGID*/> BGScore::getBGWhereDataExists()
{
	std::vector<uint32> bg_types;

	QueryResult result = CharacterDatabase.PQuery("SELECT distinct bgID FROM bgscores");
	if (!result)
		return bg_types;

	do
	{
		Field* field = result->Fetch();
		uint32 bgID = field[0].GetUInt32();

		bg_types.push_back(bgID);
	} while (result->NextRow());

	return bg_types;
}

//---------------------------------------
//Constructeur
//---------------------------------------
BGScore::BGScore(uint32 playerGUID, uint32 bgID)
{
	mPlayerGUID = playerGUID;
	mBGID		= bgID;
	mVictories  = 0;
	mDefeats    = 0;
}

//---------------------------------------
//Accesseurs
//---------------------------------------
uint32 BGScore::getPlayerGUID() const
{
	return mPlayerGUID;
}

uint32 BGScore::getBGID() const
{
	return mBGID;
}

uint32 BGScore::getVictories() const
{
	return mVictories;
}

uint32 BGScore::getDefeats() const
{
	return mDefeats;
}

double BGScore::getRatio() const
{
	if (mDefeats == 0)
		return mVictories;

	return (double)((double)mVictories / (double)mDefeats);
}

//---------------------------------------
//Interactions avec la DB
//---------------------------------------
bool BGScore::load()
{
	QueryResult result = CharacterDatabase.PQuery("SELECT victories, defeats FROM bgscores WHERE playerGUID=%u AND bgID=%u", mPlayerGUID, mBGID);
	if (!result)
		return false;

	Field* fields = result->Fetch();

	mVictories = fields[0].GetUInt32();
	mDefeats   = fields[1].GetUInt32();

	return true;
}

void BGScore::insertNewLine(uint32 victories, uint32 defeats)
{
	CharacterDatabase.PExecute("INSERT INTO bgscores VALUES(%u, %u, %u, %u)", mPlayerGUID, mBGID, victories, defeats);
}

void BGScore::update(bool victory)
{
	if (victory)
	{
		mVictories++;
	}
	else {
		mDefeats++;
	}

	if (!lineExist())
	{
		insertNewLine(mVictories, mDefeats);
		return;
	}

	CharacterDatabase.PExecute("UPDATE bgscores SET victories=%u, defeats=%u WHERE playerGUID=%u AND bgID=%u", mVictories, mDefeats, mPlayerGUID, mBGID);
}

//---------------------------------------
//Fonctions membre privées
//---------------------------------------
bool BGScore::lineExist() const
{
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM bgscores WHERE playerGUID=%u AND bgID=%u", mPlayerGUID, mBGID);
	if (result)
		return true;

	return false;
}

