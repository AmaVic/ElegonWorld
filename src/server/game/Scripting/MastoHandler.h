#ifndef DEF_MH
#define DEF_MH


#include "Player.h"
#include "Group.h"

class MastoHandler
{
	//OVERVIEW: MastoHandler gère entièrement l'événement mastodonte
	//Des triggers doivent cependant être appelés depuis l'extérieur (update depuis world)


	//TODO:- empêcher l'accès à la zone FFA pendant l'event mastodonte
	//     - choisir un nouveau mastodonte si le mastodonte se déconnecte / quitte la zone
	//     - prévoir le cas où il n'y a pas assez de joueurs dans la zone au moment de lancer l'event (reporter l'event & afficher un msg)
	//     - définir les timers (durée de l'event et temps entre deux events)
	//     - stocker plcount dans une variable au cas des joueurs partent afind de reset correctement le mastodonte
	//     - zone KS: boule qui tombe quand le joueur meurs
	//     - zone KS: définir les coordonnées de la zone shop
	//       -> note: pas mieux de faire réapparaître l'esprit des joueurs direct sur leurs cadavres ? (ne pas les rez)


public:

	//----------------------------------------
	//Constantes
	//----------------------------------------
	MastoHandler();

private:

	//----------------------------------------
	//Constantes
	//----------------------------------------
	
	//ID de l'event:
	//static const uint32 EVENT_MASTODONTE_ID;
	
	//Positions:
	static const WorldLocation POS_MASTO; //Position du mastodonte (pour le TP au lancement)
	static const WorldLocation POS_GROUP; //Position du groupe (pour TP le groupe au lancement de l'event)

	//Stats du Mastodonte: (A chaque fois, c'est multiplié par le nombre de joueurs dans le groupe)
	static const uint32 MASTO_BONUS_HP;

	//Taille
	static const float MASTO_SCALE;

	//Timers:
	static const uint32 EVENT_DURATION;			//Durée de l'event; Si le masto n'a pas été tuée à la fin de ce temps-là, il a perdu et le groupe a gagné (En millisec)
	static const uint32 EVENT_REPEAT_TIME;      //Temps entre deux events masto (en heures)

	//Nombre de joueurs requis pour l'event (masto & groupe comptés ensemble, dans la zone de l'event)
	static const uint32 EVENT_MIN_PLAYERS_COUNT;

	//Texts (Id des TrinityStrings)
	static const uint32 SAY_5MIN_BEFORE_EVENT;  //Message affiché 5 min avant le début de l'event
	static const uint32 SAY_NOT_ENOUGH_PLAYERS; //Message affiché si l'event essaye de se lancer mais qu'il n'y a pas assez de joueurs dans la zone
	static const uint32 SAY_NEXT_EVENT;		    //Message affiché pour dire dans combien de temps le prochain event aura lieu
	static const uint32 SAY_WINNER;             //Message affiché pour annoncer le gagnant de l'event

	//Honneur que les joueurs recoivent en récompense
	static const uint32 HONOR_REWARD_WINNER; //Récompense en honneur du/des gagnants
	static const uint32 HONOR_REWARD_LOSER;  //Récompense en honneur du/des perdants

	//ID de la zone
	static const uint32 AREA_ID; //ID de la zone dans laquelle se déroule l'event
	static const uint32 MAP_ID;
	static const uint32 ZONE_ID;

	//ID de l'aura à appliquer au masto (pour montrer que c'est lui)
	static const uint32 MASTO_AURA_ID;   //Id de l'aura qui désigne le masto (marque du chasseur)
	static const uint32 AURA_DAMAGE_ID;  //Id de l'aura qui augmente les dmg infligés

private:

	//----------------------------------------
	//Constantes - Enums
	//----------------------------------------
	enum Winner
	{
		WINNER_NONE,
		WINNER_MASTO,
		WINNER_GROUP
	};

private:

	//----------------------------------------
	//Fonctions membres privées
	//----------------------------------------
	float timeBeforeEventStart() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Retourne le temps (en minutes) restant avant le lancement du prochain event Masto

	bool eventCanBeStarted() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Vérifie qu'il y a assez de joueurs dans la zone de l'event
	    //         S'il y en a assez, retourne true;
	    //         Sinon, retourne false

	void choseMastodonte();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: désigne aléatoirement le mastodonte parmis les joueurs présents sur la carte

	void prepareMastodonte();
		//REQUIRES: le mastodone a été choisi
		//MODIFIES: le mastodonte
		//EFFECTS: Applique les différents boosts au mastodonte

	void prepareGroup();
		//REQUIRES: /
		//MODIFIES: le groupe
		//EFFECTS: Groupe les joueurs qui doivent vaincre le masto et les téléporte

	void sendEventStartedMessage();
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Envoie au monde entier un message disant que l'événement commence

	void rewardPlayers();
		//REQUIRES: /
		//MODIFIES: le groupe et le masto
	    //EFFECTS: les joueurs sont récompensés en fonction du fait qu'ils aient gagné / perdu

	void onEventEnd();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: réinitialise le mastodonte, récompense les joueurs, dégroupe les joueurs, planifie le prochain événement mastodonte et envoi un texte affichant le gagnant de l'event

	void planNextEvent();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: planifie le prochain événement mastodonte

public:

	//----------------------------------------
	//Fonctions membres publiques
	//----------------------------------------
	void update(uint32 diff);

	bool eventIsStarted() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne true si l'event masto est lancé, false sinon

	bool isMasto(uint64 playerGUID) const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne true si playerGUID = le masto; false sinon

	void choseNewMastoAndKeepEventGoing();
		//REQUIRES: /
		//MODIFIES:	/
		//EFFECTS: Choisit un nouveau mastodonte pour que l'event continue

	void onMastoKilled();
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: le gagnant = le group & l'event est terminé

	void resetMasto(Player* masto);
		//REQUIRES: /
		//MODIFIES: le masto
		//EFFECTS: réinitialise le mastodonte


private:

	//----------------------------------------
	//Représentation
	//----------------------------------------
	uint32  mTimer;		  //Compte le temps systématiquement afin de savoir quand lancer l'event et quand l'arrêter
	Winner  mWinner;		  //Le gagnant du combat (mastodonte ou groupe)
	bool    mEventStarted; //True si l'event est lancé, false sinon
	Player* mMastodonte;
	Group* mGroup; //Groupe de joueurs qui doivent vaincre le masto

	uint32 mMastoBaseHP;
	
	bool mMsg5minSent;

	uint32 mPlayersCountAtStart;


};

#define sMastoHandler ACE_Singleton<MastoHandler, ACE_Null_Mutex>::instance()

#endif