
#include "Player.h"

#define ONE_DAY_IN_SEC 86400

class SponsorshipHandler
{
	//OVERVIEW: SponsorshipHandler permet de g�rer enti�rement le syst�me de parrainage

public:

	//-------------------------------------------
	//M�thodes publiques statiques
	//-------------------------------------------
	static bool sponsoringPossible(uint32 sponsorAccountId, uint32 sponsoredAccountId);
		//REQUIRES: sponsorAccountId && sponsoredAccountId sont des id valides
		//MODIFIES: /
		//EFFECTS: Si le compte ayant pour id sponsorAccountId peut sponsoriser le compte ayant
		//         pour id sponsoredAccountId, retourne true; sinon: retourne false
		//         Pour qu'une relation de parrainage puisse-t-�tre lanc�e entre deux comptes 
		//         Il faut que le parrain ait (tout joueurs confondus) plus de 24h de jeu sur le serveur
		//         Et que le parrain et la parrain� soit des personnes diff�rentes (IP diff�rentes)

	static bool playerExists(const std::string& playerName);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: si un joueur portant le nom playerName existe, retourne true;
		//         sinon, retourne false

	static uint32 findAccountId(const std::string& playerName);
		//REQUIRES: le joueur qui s'appelle playerName existe
		//MODIFIES: /
		//EFFECTS: retourne l'id du compte du joueur qui s'appelle playerName

	static bool registerSponsorship(uint32 sponsorAccountId, uint32 sponsoredAccountId);
		//REQUIRES: le parrainage est � priori possible
		//MODIFIES: /
		//EFFECTS: Si aucun parrainage n'existe entre sponsorAccountId et sponsoredAccountId, cr�� le lien de parrainage et retourn true
		//         Sinon, retourne false 

	static bool areInSponsorship(uint32 firstAccountId, uint32 secondAccountId);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Si les comptes ayant pour id firstAccountId et secondAccountId sont en parrainage (dans un sens ou dans l'autre), retourne true
		//         Sinon, retourne false

	static void removeSponsorshipIfExpired();
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Supprime les parrainages expir�s

	static bool findOnlineSponsorOrSponsored(uint32 accId, uint64 commandUserGUID, uint64& guid);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Si on trouve le joueur, guid = son GUID et retourne true
		//Sinon, retourne false

	static bool isInSponsorship(uint32 accId);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne true si accId est dans un parraiange actif; false sinon

	static uint32 timeLeft(uint32 accId);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne le nombre de jours de parrainage restants � accId


};