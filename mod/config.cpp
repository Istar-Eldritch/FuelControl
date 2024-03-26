class CfgPatches
{
  class FuelControl
  {
    units[]={};
    weapons[]={};
    requiredVersion=0.1;
    requiredAddons[]=
    {
      "DZ_Data",
      "DZ_Scripts",
      "JM_CF_Scripts"
    };
  };
};

class CfgMods
{
  class FuelControl
  {
    type = "mod";

    dependencies[] = { "Game", "World", "Mission" };

    class defs
    {
	  class engineScriptModule
      {
        value = "";
        files[] = {"FuelControl/scripts/1_Core"};
      };
      class gameScriptModule
      {
        value = "";
        files[] = {"FuelControl/scripts/3_Game"};
      };

      class worldScriptModule
      {
        value = "";
        files[] = {"FuelControl/scripts/4_World"};
      };

      class missionScriptModule
      {
        value = "";
        files[] = {"FuelControl/scripts/5_Mission"};
      };
    };
  };
};

class CfgVehicles {

	class House;
	class IE_FC_ElectricalBox: House
	{
		scope=2;
		displayName="$STR_IEFC_ELECTRICAL_BOX_NAME";
		descriptionShort="$STR_IEFC_ELECTRICAL_BOX_DESC";
		model="FuelControl\data\models\IE_FC_ElectricalBox_Model.p3d";
		attachments[]= {"IE_FC_ElectricalBox"};
		class EnergyManager
		{
			hasIcon=1;
			attachmentAction=1;
			energyUsagePerSecond=0.00001;
			switchOnAtSpawn=1;
			cordTextureFile="DZ\gear\camping\Data\plug_black_CO.paa";
      		updateInterval=1;
		};
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
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};

	class Inventory_Base;
	class Edible_Base;
	class Container_Base;
	class Bottle_Base;

	class Barrel_ColorBase: Container_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class Pot: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class Cauldron: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class Canteen: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class WaterBottle: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class Vodka: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class WaterPouch_ColorBase: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}

	class CanisterGasoline: Bottle_Base {
		liquidContainerType="512 + 1024 + 2048 + 4096 + 8192 + 16384 + 65536 + 1048576";
	}
}

class CfgModels
{
	class Default
	{
		sections[] = {};
		sectionsInherit="";
		skeletonName = "";
	};
	class IE_FC_ElectricalBox_Model:Default
	{
		sections[]=
		{
      		"body"
		};

	};
};

class CfgSoundShaders {

	class baseCharacter_SoundShader;
	class IE_FC_ElectricalBox_SoundShader: baseCharacter_SoundShader {
		range = 20;
		samples[]=
		{
			
			{
				"FuelControl\data\sounds\electricity_hum",
				1
			}
		};
		volume=1;
	};

	class IE_FC_PumpRefueling_SoundShader: baseCharacter_SoundShader {
		range = 50;
		samples[]=
		{
			
			{
				"FuelControl\data\sounds\fuel_pump_loop",
				1
			}
		};
		volume=0.15;
	};

	class IE_FC_VehicleRefueling_SoundShader: baseCharacter_SoundShader {
		range = 20;
		samples[]=
		{
			
			{
				"FuelControl\data\sounds\vehicle_refuel_loop",
				1
			}
		};
		volume=1;
	};
}

class CfgSoundSets {

	class baseCharacter_SoundSet;
	class IE_FC_ElectricalBoxOnLoop_SoundSet: baseCharacter_SoundSet {
		soundShaders[]=
		{
			"IE_FC_ElectricalBox_SoundShader"
		};
	};

	class IE_FC_PumpRefueling_SoundSet: baseCharacter_SoundSet {
		soundShaders[]=
		{
			"IE_FC_PumpRefueling_SoundShader"
		};
	};

	class IE_FC_VehicleRefueling_SoundSet: baseCharacter_SoundSet {
		soundShaders[]=
		{
			"IE_FC_VehicleRefueling_SoundShader"
		};
	};
}

class cfgLiquidDefinitions
{
	class IEFCAVGasLiquid
	{
		type=1048576;
		displayName="$STR_IEFC_AVIATION_FUEL";
		flammability=60;
		class Nutrition
		{
			fullnessIndex=1;
			energy=-10;
			water=0;
			nutritionalIndex=75;
			toxicity=1;
			digestibility=2;
			agents=16;
		};
	};
};
