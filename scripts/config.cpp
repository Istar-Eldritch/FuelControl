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
	class IEFCElectricBox: House
	{
		scope=2;
		displayName="Electric Box";
		descriptionShort="Allows to power fuel stations";
		model="FuelControl\GUI\models\electric_box.p3d";
		useEntityHierarchy="true";
		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOff=1;
			attachmentAction=1;
      isPassiveDevice=1;
		};
	};
}