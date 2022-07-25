
modded class MissionBase {
	void MissionBase() {
		Print("[FuelControl] Creating mission base");
		FuelControlSettings settings = GetFuelControlSettings();
		GetRPCManager().AddRPC("FuelControl", "GetSettings", settings, SingleplayerExecutionType.Both);

		FuelStationManager manager = GetFuelStationManager();
		GetRPCManager().AddRPC("FuelControl", "UpdateStation", manager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "RequestStation", manager, SingleplayerExecutionType.Both);

		CmdManager cmdManager = GetCmdManager();
        cmdManager.RegisterHandler(new ref CmdAddStation());
        cmdManager.RegisterHandler(new ref CmdDelStation());
        cmdManager.RegisterHandler(new ref CmdGetFuel());
        cmdManager.RegisterHandler(new ref CmdGetStations());
        cmdManager.RegisterHandler(new ref CmdSetFuel());

		GetRPCManager().AddRPC("FuelControl", "HandleChatCommand", cmdManager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "HandleChatMessage", cmdManager, SingleplayerExecutionType.Both);

		if (GetGame().IsClient()) {
			settings.SyncSettings();
		}
	}
}