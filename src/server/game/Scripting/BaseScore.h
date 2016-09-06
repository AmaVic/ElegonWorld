#ifndef DEF_BASESCORE
#define DEF_BASESCORE

#include <vector>


struct AbstractScore
{
	AbstractScore(const std::string& playerName) : mPlayerName(playerName)
	{
	}

	std::string mPlayerName;
};

struct GScore : public AbstractScore
{

	GScore(const std::string& playerName, uint32 money) : AbstractScore(playerName), mMoney(money)
	{
	}

	uint32 mMoney;

};

struct KillsScore : public AbstractScore
{
	KillsScore(const std::string& playerName, uint32 plKills) : AbstractScore(playerName), kills(plKills)
	{

	}

	uint32 kills;
};


template<typename ScoreType>
class BaseScore
{

	//OVERVIEW: Un BaseScore est un classement effectué selon un critère un critère qui dépend de ScoreType
	//Un BaseScore typique est (RANKING)
	//Avec RANKING[0], RANKING[1], ..., RANKING[n] qui représentent respectivement la première, la seconde et la nème place dans le classement

public:

	//---------------------------------------------
	//Constructeur
	//---------------------------------------------
	BaseScore();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: initialise this.RANKING à l'ensemble vide

public:
		
	//---------------------------------------------
	//Fonctions membres publiques à overrider
	//---------------------------------------------
	virtual void generateRaking() = 0;
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: le classement a été calculé
		//         => this_post.RANKING contient les scores classés selon un critère qui dépend de ScoreType

	virtual std::vector<std::string> getRankingStringList() = 0;
		//REQUIRES: le classement a été généré
		//MODIFIES: /
		//EFFECTS: retourne this.RANKING convertit en une liste de chaines de caractères

	virtual void updateScore(uint64 playerGUID, uint32 amount = 1);
		//REQUIRES: /
		//MODIFIES: this.RANKING
		//EFFECTS: met à jour le score d'un joueur dans la DB (pour les classements)


public:

	//---------------------------------------------
	//Fonctions membres publiques
	//---------------------------------------------
	std::vector<ScoreType>& getRanking();
		//REQUIRES: /
		//MODIFIES: éventuellement this.RANKING (effet de bord)
		//EFFECTS: Retourne une référence vers this.RANKING




protected:

	//---------------------------------------------
	//Rep
	//---------------------------------------------
	std::vector<ScoreType> mRanking;
};

//Implémentation:
#include "BaseScore.inl"



#endif