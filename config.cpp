class CfgPatches
{
    class A1_BoostedItemSystemClient
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class CfgMods
{
    class A1_BoostedItemSystemClient
    {
        dir = "A1_BoostedItemSystemClient";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "A1_BoostedItemSystemClient";
        credits = "";
        author = "";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";
        
        dependencies[] = {"Game", "World", "Mission"};
        
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"A1_BoostedItemSystemClient/Scripts/3_Game"};
            };
            class worldScriptModule
            {
                value = "";
                files[] = {"A1_BoostedItemSystemClient/Scripts/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] = {"A1_BoostedItemSystemClient/Scripts/5_Mission"};
            };
        };
    };
};

class CfgVehicles
{
    class Inventory_Base;
    
    class A1_StoneBase: Inventory_Base
    {
        scope=0;
        displayName="$STR_A1_STONEBASE_NAME";
        descriptionShort="$STR_A1_STONEBASE_DESC";
        model="\dz\gear\consumables\SmallStone.p3d";
        debug_ItemCategory=2;
		animClass="NoFireClass";
		rotationFlags=17;
		stackedUnit="percentage";
		inventorySlot[]=
		{
			"VestHolster","VestPouch","VestGrenadeA","VestGrenadeB","VestGrenadeC","VestGrenadeD"
		};
		quantityBar=1;
		varQuantityInit=100;
		varQuantityMin=0;
		varQuantityMax=100;
		itemSize[]={1,1};
		weight=210;
		weightPerQuantityUnit=0.2;
		fragility=0.0099999998;
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=100;
                    healthLevels[]=
                    {
                        {
                            1,
                            {
                                "DZ\gear\consumables\data\stone.rvmat"
                            }
                        },
                        {
                            0.69999999,
                            {
                                "DZ\gear\consumables\data\stone.rvmat"
                            }
                        },
                        {
                            0.5,
                            {
                                "DZ\gear\consumables\data\stone_damage.rvmat"
                            }
                        },
                        {
                            0.30000001,
                            {
                                "DZ\gear\consumables\data\stone_damage.rvmat"
                            }
                        },
                        {
                            0,
                            {
                                "DZ\gear\consumables\data\stone_destruct.rvmat"
                            }
                        }
                    };
                };
            };
        };
    };
    
    class A1_StoneHealth: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONEHEALTH_NAME";
        descriptionShort="$STR_A1_STONEHEALTH_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneHealth_co.paa"
		};
    };
    
    class A1_StoneStamina: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONESTAMINA_NAME";
        descriptionShort="$STR_A1_STONESTAMINA_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneStamina_co.paa"
		};
    };
    
    class A1_StoneWater: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONEWATER_NAME";
        descriptionShort="$STR_A1_STONEWATER_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneWater_co.paa"
		};
    };
    
    class A1_StoneEnergy: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONEENERGY_NAME";
        descriptionShort="$STR_A1_STONEENERGY_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneEnergy_co.paa"
		};
    };
    
    class A1_StoneShock: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONESHOCK_NAME";
        descriptionShort="$STR_A1_STONESHOCK_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneShock_co.paa"
		};
    };
    
    class A1_StoneBlood: A1_StoneBase
    {
        scope=2;
        displayName="$STR_A1_STONEBLOOD_NAME";
        descriptionShort="$STR_A1_STONEBLOOD_DESC";
        itemSize[]={1,1};
		hiddenSelections[]= {"zbytek"};
		hiddenSelectionsTextures[]=
		{
			"A1_BoostedItemSystemClient\data\stoneBlood_co.paa"
		};
    };
};