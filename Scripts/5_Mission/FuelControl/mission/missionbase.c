
modded class MissionBase {
	void MissionBase() {
		Print("[FuelControl] Creating mission base");
		FuelControlSettings settings = GetFuelControlSettings();
		GetRPCManager().AddRPC("FuelControl", "GetSettings", settings, SingleplayerExecutionType.Both);

		FuelStationManager manager = GetFuelStationManager();
		GetRPCManager().AddRPC("FuelControl", "UpdateStation", manager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "RequestStation", manager, SingleplayerExecutionType.Both);

		CmdManager cmdManager = GetCmdManager();
		GetRPCManager().AddRPC("FuelControl", "HandleChatCommand", cmdManager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "HandleChatMessage", cmdManager, SingleplayerExecutionType.Both);

		if (GetGame().IsClient()) {
			settings.SyncSettings();
		}
	}
}