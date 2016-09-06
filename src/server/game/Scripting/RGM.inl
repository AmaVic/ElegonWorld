#include "RGM.h"

//--------------------------------------------
//Constructeur
//--------------------------------------------
template<RaidSize RAID_SIZE>
RGM<RAID_SIZE>::RGM() : mQueue(), mPlayersToGroupAndTeleport(), mPlayersToInvite()
{
	mTankCount = 0;
	mHealCount = 0;
	mDpsCount = 0;

	mTimeSinceProposal = 0;

	mProposalSent = false;

	//Composition:
	RGMComposition compo;
	switch (RAID_SIZE)
	{

		//RAID_2: used for debugs
	case RAID_2:
		compo[TANK_INDEX]	 = 0;
		compo[HEALER_INDEX]  = 0;
		compo[DPS_INDEX]	 = 2;
		break;
	case RAID_8:
		compo[TANK_INDEX]	= 1;
		compo[HEALER_INDEX] = 2;
		compo[DPS_INDEX]	= 5;
		break;

	case RAID_10:
		compo[TANK_INDEX]	= 2;
		compo[HEALER_INDEX] = 2;
		compo[DPS_INDEX]	= 6;
		break;

	case RAID_25:
		compo[TANK_INDEX]	= 3;
		compo[HEALER_INDEX] = 5;
		compo[DPS_INDEX]	= 12;
		break;
	}

	mCompo = compo;
}

//--------------------------------------------
//Méthodes publiques
//--------------------------------------------
template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::addPlayerInQueue(PlayerGUID guid, PlayerRole playerRole)
{
	auto inserted = mQueue.insert(std::make_pair(guid, playerRole));
	std::string str;

	if (inserted.second)
		str = "Vous etes bien inscrit dans la recherche de raid en tant que " + playerRoleToString(playerRole);
	//Sinon:
	else
		str = "Erreur: impossible de vous inscrire dans la recherche de raid";

	//On dit au joueur qu'il est bien dans la recherche
	Player* p = sObjectAccessor->FindPlayer(guid);
	if (!p)
		return;
	sendMessageToPlayer(guid, COLOR_YELLOW, str.c_str());

	//On met à jour les rôles des joueurs présents dans la file
	updateRoleCount();

	//On regarde s'il est possible de faire un groupe et si oui on envoie les invitations
	if (groupCanBeMade())
	{
		chosePlayersToInvite();
		sendProposals();
	}

}

template<RaidSize RAID_SIZE>
bool RGM<RAID_SIZE>::isPlayerInQueue(PlayerGUID guid) const
{
	auto found = mQueue.find(guid);
	return found != mQueue.end();
}

template<RaidSize RAID_SIZE>
bool RGM<RAID_SIZE>::isPlayerSelected(PlayerGUID guid) const
{
	for (std::size_t i = 0; i < mPlayersToInvite.size(); i++)
	{
		if (mPlayersToInvite.at(i) == guid)
			return true;
	}

	return false;
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::removePlayerFromQueue(PlayerGUID guid)
{
	int queueSize = mQueue.size();
	mQueue.erase(guid);
	if (mQueue.size() != queueSize)
		updateRoleCount();

	resetPlayerProposal(guid);
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::onUpdate(uint32 diff)
{
	if (mQueue.empty())
		return;

	//A ACTIVER POUR LE MODE RELEASE
	if (mQueue.size() < RAID_SIZE)
		return;

	if (mProposalSent)
	{
		mTimeSinceProposal += diff;


		//Màj des RGMProposal
		Player* currentPlayer = nullptr;
		for (auto it = mQueue.begin(); it != mQueue.end(); ++it)
		{
			currentPlayer = sObjectAccessor->FindPlayer(it->first);
			if (!currentPlayer)
				continue;

			//On ajoute le temps écoulé
			Player::RGMProposal& proposal = currentPlayer->getProposal();
			if (!proposal.inProgress)
				continue;

			proposal.elapsedTimeSinceProposal += diff;

			//DEBUG:
			//currentPlayer->GetSession()->SendAreaTriggerMessage("Temps depuis la proposition: %u", proposal.elapsedTimeSinceProposal);

		}

		//Si le temps est écoulé, on récupère le nombre de réponses favorables
		//Si tous les joueurs ont accepté, ils sont groupés et téléportés pour le raid et sont exclus de la recherche de raid
		//Sinon, les joueurs ayant accepté restent dans la file d'attente et les autres en sont exclus
		if (mTimeSinceProposal >= TIME_TO_VOTE)
		{
			//Si il n'y a pas assez de réponses favorables
			int repOk = getAcceptInviteCount();
			if (repOk < RAID_SIZE)
			{
				//On exclus les joueurs ayant refusé de la recherche
				for (auto it = mPlayersToInvite.begin(); it != mPlayersToInvite.end(); ++it)
				{
					Player* player = sObjectAccessor->FindPlayer(*it);
					if (!player)
						continue;
					if (player->getProposal().answer)
					{
						resetPlayerProposal(*it);
						player->GetSession()->SendAreaTriggerMessage("Certains joueurs ont refusé l'invitation de recherche de raid, merci de patienter");
						sendMessageToPlayer(*it, COLOR_RED, "Certains joueurs ont refuse l'invitation de recherche de raid, merci de patienter");
						continue;
					}

					resetPlayerProposal(*it);
					removePlayerFromQueue(*it);
					sendMessageToPlayer(*it, COLOR_RED, "Vous n'etes plus dans la file d'attente pour la recherche de raid");
					player->GetSession()->SendNotification("Vous n'etes plus dans la file d'attente pour la recherche de raid");

				}

				//Clean mPlayersToInvite
				mPlayersToInvite.fill(0);

				//Il n'y a plus de propositions en cours...
				mProposalSent = false;

				//On reset le timer
				mTimeSinceProposal = 0;


			}
			else { //Si tous les joueurs invités ont répondu oui...

				//On les ajoute dans les joueurs à grouper et à téléporter et on les supprime de la liste des joueurs à inviter
				int index = 0;
				for (auto it = mPlayersToInvite.begin(); it != mPlayersToInvite.end(); ++it)
				{
					mPlayersToGroupAndTeleport[index] = *it;
					mPlayersToInvite[index] = 0;
					index++;
				}

				//On ajoute les joueurs dans le groupe, on les tele, on les supprime de la file et des joueurs à grouper et à tele
				Group* group = new Group();
				group->ConvertToRaid();
				group->SetLootMethod(NEED_BEFORE_GREED);
				index = 0;
				for (auto it = mPlayersToGroupAndTeleport.begin(); it != mPlayersToGroupAndTeleport.end(); ++it)
				{
					Player* p = sObjectAccessor->FindPlayer(*it);
					if (!p)
						continue;

					p->RemoveFromGroup();

					group->AddMember(p);

					p->AddAura(RGM_AURA, p);

					p->TeleportTo(ZG_MAP_ID, ZG_X, ZG_Y, ZG_Z, ZG_O);
					resetPlayerProposal(*it);
					removePlayerFromQueue(*it);
					mPlayersToGroupAndTeleport[index] = 0;
					index++;
				}
				//Il n'y a plus de propositions envoyées et le timer est reset
				mTimeSinceProposal = 0;
				mProposalSent = false;


			}

		}

	}
}

//--------------------------------------------
//Méthodes privées
//--------------------------------------------

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::sendMessageToPlayer(PlayerGUID guid, const char* color, const char* msg) const
{
	std::string str;
	str = std::string(color) + " " + std::string(msg);

	WorldPacket data;
	Player* player = sObjectAccessor->FindPlayer(guid);
	if (!player)
		return;

	ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, Language(0), player, player, str);
	player->SendMessageToSetInRange(&data, 0, true);
	
}

template<RaidSize RAID_SIZE>
std::string RGM<RAID_SIZE>::playerRoleToString(PlayerRole role) const
{
	switch (role)
	{
	case ROLE_TANK:
		return "Tank";
		break;
	case ROLE_HEAL:
		return "Heal";
		break;
	case ROLE_DPS:
		return "DPS";
		break;
	}
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::updateRoleCount()
{
	int tankCount = 0;
	int healCount = 0;
	int dpsCount = 0;

	for (auto it = mQueue.begin(); it != mQueue.end(); ++it)
	{
		PlayerRole currentRole = it->second;

		switch (currentRole)
		{
		case ROLE_TANK:
			tankCount++;
			break;
		case ROLE_HEAL:
			healCount++;
			break;
		case ROLE_DPS:
			dpsCount++;
			break;
		}
	}

	mTankCount = tankCount;
	mHealCount = healCount;
	mDpsCount = dpsCount;
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::sendProposalToPlayer(PlayerGUID guid) const
{
	Player* player = sObjectAccessor->FindPlayer(guid);
	if (!player)
		return;

	Player::RGMProposal& plProposal = player->getProposal();
	plProposal.inProgress = true;
	plProposal.answer = false;

	player->GetSession()->SendNotification("Tappez .raid oui pour rejoindre le groupe de raid \n Tappez .raid non pour quitter la file");
	sendMessageToPlayer(guid, COLOR_GREEN, "Tappez .raid oui pour rejoindre le groupe de raid");
	sendMessageToPlayer(guid, COLOR_GREEN, "Tappez .raid non pour quitter la file");
}

template<RaidSize RAID_SIZE>
bool RGM<RAID_SIZE>::groupCanBeMade() const
{
	return mTankCount >= mCompo[TANK_INDEX] && mHealCount >= mCompo[HEALER_INDEX] && mDpsCount >= mCompo[DPS_INDEX];
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::chosePlayersToInvite()
{
	
	int tankCount = 0;
	int healCount = 0;
	int dpsCount  = 0;

	int index = 0;

	for(auto it = mQueue.begin(); it != mQueue.end() && (tankCount + healCount + dpsCount) < RAID_SIZE; ++it)
	{
		if(it->second == ROLE_TANK)
			tankCount++;
		if(it->second == ROLE_HEAL)
			tankCount++;
		if(it->second == ROLE_DPS)
			dpsCount++;

		mPlayersToInvite[index] = it->first;

		index++;
	}
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::sendProposals()
{
	for(auto it = mPlayersToInvite.begin(); it != mPlayersToInvite.end(); ++it)
		sendProposalToPlayer(*it);

	mProposalSent = true;
}

template<RaidSize RAID_SIZE>
int RGM<RAID_SIZE>::getAcceptInviteCount() const
{
	int count = 0;
	for(auto it = mPlayersToInvite.begin(); it != mPlayersToInvite.end(); ++it)
	{
		Player* currentPlayer = sObjectAccessor->FindPlayer(*it);
		if(!currentPlayer)
			continue;
		if(currentPlayer->getProposal().answer)
			count++;
	}

	return count;
}

template<RaidSize RAID_SIZE>
void RGM<RAID_SIZE>::resetPlayerProposal(PlayerGUID guid) const	
{
	Player* p = sObjectAccessor->FindPlayer(guid);
	if(!p)
		return;

	Player::RGMProposal& prop = p->getProposal();
	prop.answer = false;
	prop.elapsedTimeSinceProposal = 0;
	prop.inProgress = false;
}





