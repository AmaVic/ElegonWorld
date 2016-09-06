#include "ScriptMgr.h"


class script_remove_character : public PlayerScript {

public:

	script_remove_character() : PlayerScript("script_remove_character"){}

	void OnDelete(uint64 guid, uint32 accountId) {

		CharacterDatabase.PExecute("DELETE IGNORE FROM bgscores WHERE playerGUID = %u", guid);
		CharacterDatabase.PExecute("DELETE IGNORE FROM ffarankings WHERE playerGUID = %u", guid);
		CharacterDatabase.PExecute("UPDATE house_buy SET owner = 0 WHERE owner = %u", guid);
		CharacterDatabase.PExecute("DELETE IGNORE FROM loe_ranking WHERE playerGUID = %u", guid);
		CharacterDatabase.PExecute("DELETE IGNORE FROM zone_player WHERE guid = %u", guid);
	}
};

void AddSC_script_remove_character()
{
	new script_remove_character();
}