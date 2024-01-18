class CfgPatches
{
  class FC_Scripts
  {
    units[]={};
    weapons[]={};
    requiredVersion=0.1;
    requiredAddons[]=
    {
      "DZ_Data",
      "DZ_Scripts",
      "JM_CF_Scripts",
      "FC_GUI"
    };
  };
};

class CfgMods
{
  class FC_Scripts
  {
    type = "mod";

    dependencies[] = { "Game", "World", "Mission" };

    class defs
    {
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
		displayName="Electric Box";
		descriptionShort="Allows to power fuel stations";
		model="FuelControl\GUI\models\IE_FC_ElectricalBox_Model.p3d";
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
/*<potential axis>
boundingbox_max
boundingbox_min
ce_center
ce_radius
invview
</potential axis>*/
	};//</Modelclass>
};//</CfgModels>

class CfgSoundShaders {

	class baseCharacter_SoundShader;
	class IE_FC_ElectricalBox_SoundShader: baseCharacter_SoundShader {
		range = 20;
		samples[]=
		{
			
			{
				"FuelControl\GUI\sounds\electricity_hum",
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
				"FuelControl\GUI\sounds\fuel_pump_loop",
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
				"FuelControl\GUI\sounds\vehicle_refuel_loop",
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