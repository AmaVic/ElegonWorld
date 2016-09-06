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

	//OVERVIEW: Un BaseScore est un classement effectu� selon un crit�re un crit�re qui d�pend de ScoreType
	//Un BaseScore typique est (RANKING)
	//Avec RANKING[0], RANKING[1], ..., RANKING[n] qui repr�sentent respectivement la premi�re, la seconde et la n�me place dans le classement

public:

	//---------------------------------------------
	//Constructeur
	//---------------------------------------------
	BaseScore();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: initialise this.RANKING � l'ensemble vide

public:
		
	//---------------------------------------------
	//Fonctions membres publiques � overrider
	//---------------------------------------------
	virtual void generateRaking() = 0;
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: le classement a �t� calcul�
		//         => this_post.RANKING contient les scores class�s selon un crit�re qui d�pend de ScoreType

	virtual std::vector<std::string> getRankingStringList() = 0;
		//REQUIRES: le classement a �t� g�n�r�
		//MODIFIES: /
		//EFFECTS: retourne this.RANKING convertit en une liste de chaines de caract�res

	virtual void updateScore(uint64 playerGUID, uint32 amount = 1);
		//REQUIRES: /
		//MODIFIES: this.RANKING
		//EFFECTS: met � jour le score d'un joueur dans la DB (pour les classements)


public:

	//---------------------------------------------
	//Fonctions membres publiques
	//---------------------------------------------
	std::vector<ScoreType>& getRanking();
		//REQUIRES: /
		//MODIFIES: �ventuellement this.RANKING (effet de bord)
		//EFFECTS: Retourne une r�f�rence vers this.RANKING




protected:

	//---------------------------------------------
	//Rep
	//---------------------------------------------
	std::vector<ScoreType> mRanking;
};

//Impl�mentation:
#include "BaseScore.inl"



#endif