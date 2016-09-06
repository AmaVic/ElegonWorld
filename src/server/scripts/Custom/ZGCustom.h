

#define ZGCustomScriptName "instance_zulgurub_custom"

enum DataTypes
{
	DATA_ZG_1 = 0 //Premier BOSS
};

template<class AI>
AI* GetZGCustomAI(Creature* creature)
{
	return GetInstanceAI<AI>(creature, ZGCustomScriptName);
}