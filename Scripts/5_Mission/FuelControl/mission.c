
modded class MissionBase
{
	void MissionBase()
	{
		Print("[FuelControl] Creating mission base");
		FuelStationManager manager = GetFuelStationManager();
		GetRPCManager().AddRPC("FuelControl", "GetFuelStations", manager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "UpdateStation", manager, SingleplayerExecutionType.Both);

		if (GetGame().IsClient()) {
			manager.SyncFuelStations();
		}
	}

	void TestRPCFunction(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<string> data;
		if (!ctx.Read(data)) return;

		if (type == CallType.Server)
		{
			Print("Function called on server!");
			GetRPCManager().SendRPC("FuelControl", "TestRPCFunction", new Param1<string>("Hello, World!"));  
		}
		else
		{
			Print("Function called on client!");
		}
	}

}