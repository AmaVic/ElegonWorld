#ifndef DEF_GOLDSCORE
#define DEG_GOLDSCORE

#include "BaseScore.h"

class GoldScore : public BaseScore<GScore>
{

	//OVERVIEW: un GoldScore est un classement en fonction des pi�ces d'or du joueur

public:

	//---------------------------------------------
	//Constructeur
	//---------------------------------------------
	GoldScore();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: initialise this � l'ensemble vide
public:

	//---------------------------------------------
	//Fonctions membres publiques � overrider
	//---------------------------------------------
	virtual void generateRaking();
		//Voir doc dans la classe m�re

	virtual std::vector<std::string> getRankingStringList();
		//Voir doc dans la classe m�re

	virtual void updateScore(uint64 playerGUID, uint32 amount = 1);
		//voir doc de la classe m�re


};

#endif