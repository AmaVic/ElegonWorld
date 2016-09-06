#ifndef DEF_MH
#define DEF_MH


#include "Player.h"
#include "Group.h"

class MastoHandler
{
	//OVERVIEW: MastoHandler g�re enti�rement l'�v�nement mastodonte
	//Des triggers doivent cependant �tre appel�s depuis l'ext�rieur (update depuis world)


	//TODO:- emp�cher l'acc�s � la zone FFA pendant l'event mastodonte
	//     - choisir un nouveau mastodonte si le mastodonte se d�connecte / quitte la zone
	//     - pr�voir le cas o� il n'y a pas assez de joueurs dans la zone au moment de lancer l'event (reporter l'event & afficher un msg)
	//     - d�finir les timers (dur�e de l'event et temps entre deux events)
	//     - stocker plcount dans une variable au cas des joueurs partent afind de reset correctement le mastodonte
	//     - zone KS: boule qui tombe quand le joueur meurs
	//     - zone KS: d�finir les coordonn�es de la zone shop
	//       -> note: pas mieux de faire r�appara�tre l'esprit des joueurs direct sur leurs cadavres ? (ne pas les rez)


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

	//Stats du Mastodonte: (A chaque fois, c'est multipli� par le nombre de joueurs dans le groupe)
	static const uint32 MASTO_BONUS_HP;

	//Taille
	static const float MASTO_SCALE;

	//Timers:
	static const uint32 EVENT_DURATION;			//Dur�e de l'event; Si le masto n'a pas �t� tu�e � la fin de ce temps-l�, il a perdu et le groupe a gagn� (En millisec)
	static const uint32 EVENT_REPEAT_TIME;      //Temps entre deux events masto (en heures)

	//Nombre de joueurs requis pour l'event (masto & groupe compt�s ensemble, dans la zone de l'event)
	static const uint32 EVENT_MIN_PLAYERS_COUNT;

	//Texts (Id des TrinityStrings)
	static const uint32 SAY_5MIN_BEFORE_EVENT;  //Message affich� 5 min avant le d�but de l'event
	static const uint32 SAY_NOT_ENOUGH_PLAYERS; //Message affich� si l'event essaye de se lancer mais qu'il n'y a pas assez de joueurs dans la zone
	static const uint32 SAY_NEXT_EVENT;		    //Message affich� pour dire dans combien de temps le prochain event aura lieu
	static const uint32 SAY_WINNER;             //Message affich� pour annoncer le gagnant de l'event

	//Honneur que les joueurs recoivent en r�compense
	static const uint32 HONOR_REWARD_WINNER; //R�compense en honneur du/des gagnants
	static const uint32 HONOR_REWARD_LOSER;  //R�compense en honneur du/des perdants

	//ID de la zone
	static const uint32 AREA_ID; //ID de la zone dans laquelle se d�roule l'event
	static const uint32 MAP_ID;
	static const uint32 ZONE_ID;

	//ID de l'aura � appliquer au masto (pour montrer que c'est lui)
	static const uint32 MASTO_AURA_ID;   //Id de l'aura qui d�signe le masto (marque du chasseur)
	static const uint32 AURA_DAMAGE_ID;  //Id de l'aura qui augmente les dmg inflig�s

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
	//Fonctions membres priv�es
	//----------------------------------------
	float timeBeforeEventStart() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Retourne le temps (en minutes) restant avant le lancement du prochain event Masto

	bool eventCanBeStarted() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: V�rifie qu'il y a assez de joueurs dans la zone de l'event
	    //         S'il y en a assez, retourne true;
	    //         Sinon, retourne false

	void choseMastodonte();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: d�signe al�atoirement le mastodonte parmis les joueurs pr�sents sur la carte

	void prepareMastodonte();
		//REQUIRES: le mastodone a �t� choisi
		//MODIFIES: le mastodonte
		//EFFECTS: Applique les diff�rents boosts au mastodonte

	void prepareGroup();
		//REQUIRES: /
		//MODIFIES: le groupe
		//EFFECTS: Groupe les joueurs qui doivent vaincre le masto et les t�l�porte

	void sendEventStartedMessage();
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: Envoie au monde entier un message disant que l'�v�nement commence

	void rewardPlayers();
		//REQUIRES: /
		//MODIFIES: le groupe et le masto
	    //EFFECTS: les joueurs sont r�compens�s en fonction du fait qu'ils aient gagn� / perdu

	void onEventEnd();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: r�initialise le mastodonte, r�compense les joueurs, d�groupe les joueurs, planifie le prochain �v�nement mastodonte et envoi un texte affichant le gagnant de l'event

	void planNextEvent();
		//REQUIRES: /
		//MODIFIES: this
		//EFFECTS: planifie le prochain �v�nement mastodonte

public:

	//----------------------------------------
	//Fonctions membres publiques
	//----------------------------------------
	void update(uint32 diff);

	bool eventIsStarted() const;
		//REQUIRES: /
		//MODIFIES: /
		//EFFECTS: retourne true si l'event masto est lanc�, false sinon

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
		//EFFECTS: le gagnant = le group & l'event est termin�

	void resetMasto(Player* masto);
		//REQUIRES: /
		//MODIFIES: le masto
		//EFFECTS: r�initialise le mastodonte


private:

	//----------------------------------------
	//Repr�sentation
	//----------------------------------------
	uint32  mTimer;		  //Compte le temps syst�matiquement afin de savoir quand lancer l'event et quand l'arr�ter
	Winner  mWinner;		  //Le gagnant du combat (mastodonte ou groupe)
	bool    mEventStarted; //True si l'event est lanc�, false sinon
	Player* mMastodonte;
	Group* mGroup; //Groupe de joueurs qui doivent vaincre le masto

	uint32 mMastoBaseHP;
	
	bool mMsg5minSent;

	uint32 mPlayersCountAtStart;


};

#define sMastoHandler ACE_Singleton<MastoHandler, ACE_Null_Mutex>::instance()

#endif