class CfgPatches
{
	class FuelControl
	{
		requiredAddons[]=
		{
			"CF"
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