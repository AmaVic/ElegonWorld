
#include "RiftHandler.h"

//-----------------------------------------------
//Constructeur
//-----------------------------------------------
RiftHandler::RiftHandler() : mCurrentRiftLocation(), mData(), mPlayersInGroup(0)
{
	mIsUnderAttack = false;
	mIsOpen = false;
	mIsUnderAttack = false;
	mIsSummoned = false;

	//Data:
	mData.insert(std::make_pair(256891, 66));
	mData.insert(std::make_pair(256892, 394));
	mData.insert(std::make_pair(256914, 4127));
	mData.insert(std::make_pair(256973, 4553));
	mData.insert(std::make_pair(256985, 922));
	mData.insert(std::make_pair(256993, 1739));
	mData.insert(std::make_pair(257003, 2562));
	mData.insert(std::make_pair(257004, 4));
	mData.insert(std::make_pair(257007, 300));
	mData.insert(std::make_pair(257008, 38));
	mData.insert(std::make_pair(257010, 380));
	mData.insert(std::make_pair(257011, 222));
	mData.insert(std::make_pair(257012, 431));
	mData.insert(std::make_pair(257018, 1766));
	mData.insert(std::make_pair(257020, 2251));
	mData.insert(std::make_pair(257019, 2244));

}





//-----------------------------------------------
//Méthodes privées
//-----------------------------------------------


void RiftHandler::RewardPlayers()
{
}
uint64 RiftHandler::ChoseRandomCreatureGUID(uint64 lastCreatureGUID) const
{
	auto it = mData.begin();
	std::advance(it, urand(0, mData.size() - 1));

	return it->first;
}

//-----------------------------------------------
//OnEvent
//-----------------------------------------------

void RiftHandler::OnRiftClosed(uint64 creatureGUID)
{

	//Récompense des joueurs:
	RewardPlayers();

	//La faille n'est plus attaquée:
	mIsUnderAttack = false;
	mIsOpen = false;
	mIsSummoned = false;

	mTs->DespawnOrUnsummon();

}

void RiftHandler::OnPlayerGettingClose()
{
	Map * map = sMapMgr->FindMap(mCurrentRiftLocation.m_mapId, 0);
	mTs = map->SummonCreature(1500001, mCurrentRiftLocation);
	mLastCreature = mTs->ToCreature();
	mLastCreature->setFaction(16);
	mLastCreature->AddAura(RIFT_AURA, mLastCreature);
	mIsSummoned = true;
}

void RiftHandler::OnPlayerGettingAway(Player* player)
{
	
}

//-----------------------------------------------
//Accesseurs
//-----------------------------------------------


//-----------------------------------------------
//Méthodes publiques
//-----------------------------------------------
void RiftHandler::OpenNewRift(Player* player, uint64 creatureGUID)
{

	uint64 newCreatureGUID = ChoseRandomCreatureGUID(0);
	QueryResult result = WorldDatabase.PQuery("SELECT map, position_x, position_y, position_z, orientation FROM creature WHERE guid=%u", newCreatureGUID);
	if (!result)
	{
		return;
	}

	Field* fields = result->Fetch();


	mCurrentRiftLocation.m_mapId = fields[0].GetUInt32();
	mCurrentRiftLocation.m_positionX = fields[1].GetFloat();
	mCurrentRiftLocation.m_positionY = fields[2].GetFloat();
	mCurrentRiftLocation.m_positionZ = fields[3].GetFloat();
	mCurrentRiftLocation.m_orientation = fields[4].GetFloat();


	//Notification des joueurs
	uint32 areaId = mData.find(newCreatureGUID)->second;
	AreaTableEntry const* data = GetAreaEntryByAreaID(areaId);
	mAreaId = areaId;
	//AreaTableEntry const* zone = GetAreaEntryByAreaID(data->zone);

	std::string name = std::string(data->area_name[LOCALE_frFR]);
	//std::string zoneName = std::string(zone->area_name[LOCALE_frFR]);
	mAreaName = name;
	//mZoneName = zoneName;
	mLastCreatureGUID = newCreatureGUID;
	sWorld->SendWorldText(TEXT_NEW_RIFT, name.c_str());

	mIsOpen = true;
	mIsSummoned = false;
}

uint64 RiftHandler::GetLastCreatureGUID() const
{
	return mLastCreatureGUID;
}

bool RiftHandler::IsOpen() const
{
	return mIsOpen;
}

Position RiftHandler::GetRiftPosition() const
{
	return mLastCreature->GetPosition();
}

bool RiftHandler::IsUnderAttack() const
{
	return mIsUnderAttack;
}

uint32 RiftHandler::GetAreaId() const
{
	return mAreaId;
}

void RiftHandler::SetUnderAttack(bool ua)
{
	mIsUnderAttack = ua;
}

std::string RiftHandler::GetAreaName() const
{
	return mAreaName;
}

std::string RiftHandler::GetZoneName() const
{
	return mZoneName;
}

bool RiftHandler::IsSummoned() const
{
	return mIsSummoned;
}
