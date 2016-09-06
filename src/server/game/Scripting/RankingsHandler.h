
#ifndef DEF_RANKINGSHANDLER
#define DEF_RANKINGSHANDLER

#include "BaseScore.h"
#include "GoldScore.h"
#include "VHScore.h"

#include <array>

class RankingsHandler
{

public:

	//---------------------------------------------
	//Constructeur
	//---------------------------------------------
	RankingsHandler();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: initialise tous les scores à 0

public:

	//---------------------------------------------
	//Enums
	//---------------------------------------------
	enum Scores
	{
		GOLD_SCORE = 0,
		VH_SCORE = 1,

		SCORE_COUNT = 2
	};

private:

	GoldScore mGS;
	VHScore   mVH;


};

#endif