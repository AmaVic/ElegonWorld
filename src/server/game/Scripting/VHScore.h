

#include "BaseScore.h"

class VHScore : public BaseScore<KillsScore>
{

public:

	//---------------------------------------------
	//Constructeur
	//---------------------------------------------
	VHScore();

public:

	//---------------------------------------------
	//Fonctions membres publiques � overrider
	//---------------------------------------------
	virtual void generateRaking();

	virtual std::vector<std::string> getRankingStringList();
};