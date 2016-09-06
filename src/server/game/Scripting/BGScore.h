
#ifndef DEF_BGSCORE
#define DEF_BGSCORE

class BGScore
{
	//OVERVIEW: un BGScore représente le score d'un joueur (victoires - défaites) dans un champ de bataille
	//Un BGScore typique est (PLAYER_GUID, BG_ID, VICTORIES, DEFEATS

	//Mutable

private:
	
	//---------------------------------------
	//Représentation
	//---------------------------------------
	uint32 mPlayerGUID;
	uint32 mBGID;
	uint32 mVictories;
	uint32 mDefeats;

public:

	//---------------------------------------
	//Fonctions publiques statiques
	//---------------------------------------
	static std::vector<BGScore> getBGScores(uint32 bg_id);
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Charge tous les BGScores enregistrés dans la DB

	static std::vector<uint32 /*BGID*/> getBGWhereDataExists();
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne une liste des BG pour lesquels des données existent

public:

	//---------------------------------------
	//Constructeur
	//---------------------------------------
	BGScore(uint32 playerGUID, uint32 bgID);
		//REQUIRES: playerGUID et bgID sont tous les deux valides
		//MODIFIES: this
		//EFFECTS: this_post.PLAYER_GUID = playerGUID
	    //      && this_post.BG_ID = bgID
		//      && this_post.VICTORIES = this_post.DEFEATS = 0

private:

	//---------------------------------------
	//Fonctions membre privées
	//---------------------------------------
	bool lineExist() const;
		//EFFECTS: si il existe une ligne ayant pour identifiant (PLAYER_GUID, BG_ID), retourne true; sinon, retourne false

public:

	//---------------------------------------
	//Accesseurs
	//---------------------------------------
	uint32 getPlayerGUID() const;
		//EFFECTS: retourne this.PLAYER_GUID

	uint32 getBGID() const;
		//EFFECTS: retourne this.BG_ID

	uint32 getVictories() const;
		//REQUIRES: this a été chargé
		//EFFECTS: retourne this.VICTORIES

	uint32 getDefeats() const;
		//REQUIRES: this a été chargé
		//EFFECTS: retourne this.DEFEATS

	double getRatio() const;
		//REQUIRES: this a été chargé
		//EFFECTS: si this.DEFEATS == 0, retourne this.VICTORIES
	    //         sinon, retourne this.VICTORIES / this.DEFEATS

	//---------------------------------------
	//Interactions avec la DB
	//---------------------------------------
	bool load();
		//MODIFIES: this
		//EFFECTS: charge les données pour le joueur PLAYER_GUID et pour le BG BG_ID
	    //         dans le cas ou les données n'ont pas pu être trouvées, retourne false; dans le cas contraire, retourne true

	void insertNewLine(uint32 victories, uint32 defeats);
		//REQUIRES: aucune ligne n'ayant pour identifiant (PLAYER_GUID, BG_ID) n'existe dans la table
		//MODIFIES: this && la DB
		//EFFECTS: insère dans la base de donnée une nouvelle ligne ayant pour identifiant (PLAYER_GUID, BG_ID) et les nombres de victoires et de défaites sont respectivement (victories, defeats)
	    //     &&  this_post.VICTORIES = victories
	    //     &&  this_post.DEFEATS = defeats

	void update(bool victory);
		//REQUIRES: une ligne ayant pour identifiant (PLAYER_GUID, BG_ID) existe dans la table
		//MODIFIES: this && la DB
		//EFFECTS: si victory == true => (this_post.VICTORIES++ && on met à jour la DB pour ajouter la victoire)
		//         sinon, this_post.DEFEATS++ && on met à jour la DB pour ajouter la défaite
};

#endif