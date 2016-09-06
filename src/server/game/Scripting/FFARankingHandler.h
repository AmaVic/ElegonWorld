
#include <vector>

class FFARankingHandler
{

	/**
	@overview: FFARankingHandler contient un ensemble de fonctions permettant d'enregistrer les statistiques des joueurs
	dans la zone FFA ainsi que des fonctions de déterminer classements liées à ces statistiques
	*/

public:

	//----------------------------------------
	//Scores
	//----------------------------------------
	struct FFAScore
	{
		uint64 guid;   //player's GUID
		uint32 kills;  //player's kills in FFA area
		uint32 deaths; //player's deaths in FFA area
		double ratio;  //player's ratio in FFA area
		uint32 maxKS;  //player's max KS in FFA area
	};


public:

	//----------------------------------------
	//enums
	//----------------------------------------
	enum OrderCriteria
	{
		BY_KILLS = 0,
		BY_RATIO = 1,
		BY_MAX_KILLSTREAK = 2
	};

public:

	//----------------------------------------
	//Typedefs
	//----------------------------------------
	typedef std::vector<FFAScore> Scores;

	//----------------------------------------
	//Fonctions publiques statiques
	//----------------------------------------
	static void registerPvPKill(uint64 killerGUID, uint64 killedGUID);
		//REQUIRES: /
		//MODIFIES: characters.ffarankings (DB)
		//EFFECTS: ajoute 1 aux nombres de victimes de killerGUID dans la zone FFA et ajoute un décès à killedGUID
		//         et met à jour la série d'élimination la plus élevée du tueur

	static Scores generateRanking(OrderCriteria order);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne une liste des scores ordonnée par order

	static OrderCriteria strToOrderCriteria(const char* str);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne le critère de classement correspondant à str
};