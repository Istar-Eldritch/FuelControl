
modded class MissionBase {
	void MissionBase() {
		Print("[FuelControl] Creating mission base");
		FuelStationManager manager = GetFuelStationManager();
		GetRPCManager().AddRPC("FuelControl", "GetFuelStations", manager, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("FuelControl", "UpdateStation", manager, SingleplayerExecutionType.Both);

		if (GetGame().IsClient()) {
			manager.SyncFuelStations();
		}
	}

}