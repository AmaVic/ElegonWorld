

//---------------------------------------------
//Constructeur
//---------------------------------------------
template<typename ScoreType>
BaseScore<typename ScoreType>::BaseScore() : mRanking()
{

}

//---------------------------------------------
//Fonctions membres publiques
//---------------------------------------------
template<typename ScoreType>
std::vector<ScoreType>& BaseScore<ScoreType>::getRanking()
{
	return mRanking;
}

template<typename ScoreType>
void BaseScore<ScoreType>::updateScore(uint64 playerGUID, uint32 amount)
{
	QueryResult result = CharacterDatabase.PQuery("SELECT * FROM rankings WHERE guid=%u", playerGUID);
	if (result)
		return;

	CharacterDatabase.PExecute("INSERT INTO rankings VALUES (%u, 0, 0, 0, 0, 0, 0, 0)", playerGUID);
}