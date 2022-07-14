
modded class MissionBase {
	void MissionBase() {
		Print("[FuelControl] Creating mission base");
		FuelControlSettings settings = GetFuelControlSettings();
		GetRPCManager().AddRPC("FuelControl", "GetSettings", settings, SingleplayerExecutionType.Both);

		FuelStationManager manager = GetFuelStationManager();
		GetRPCManager().AddRPC("FuelControl", "UpdateStation", manager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "RequestStation", manager, SingleplayerExecutionType.Both);


		if (GetGame().IsClient()) {
			settings.SyncSettings();
		}
	}

}