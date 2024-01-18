
modded class MissionBase {
  void MissionBase() {
	#ifdef DIAG_DEVELOPER
	CF_Log.Level = CF_LogLevel.TRACE;
	#endif
    FuelControlSettings settings = GetFuelControlSettings();
    GetRPCManager().AddRPC("IE_FC", "FuelControlSettingsOnSyncRPC", settings, SingleplayerExecutionType.Both);
	GetRPCManager().AddRPC("IE_FC", "FuelControlSettingsDeleteSyncRPC", settings, SingleplayerExecutionType.Both);

    FuelStationManager manager = GetFuelStationManager();
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncAll", manager, SingleplayerExecutionType.Both);
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncStation", manager, SingleplayerExecutionType.Both);
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncDeleteStation", manager, SingleplayerExecutionType.Both);

    FCTeleportManager teleportManager = FCGetTeleportManager();
    GetRPCManager().AddRPC("IE_FC", "TeleportToStation", teleportManager, SingleplayerExecutionType.Both);

	GetRPCManager().AddRPC("IE_FC", "FuelStationSoundUpdate", this, SingleplayerExecutionType.Both);

    if (GetGame().IsClient()) {
      settings.SyncAll();
	  manager.SyncAll();
    }

    if (GetGame().IsServer()) {
      if(settings.settings.spawn_enabled) {
        GetFuelSpawner().Start();
      }
    }
  }
	
  override void OnMissionLoaded() {
	if(GetGame().IsServer()) {
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GeneratePowerSources, 1000);
	}
  }
	
  void GeneratePowerSources() {
	  CF_Log.Debug("[FuelControl] Generating power sources");
	  auto settings = GetFuelControlSettings();
	  // Spawn power boxes
	  foreach (auto box: settings.power_boxes) {
		  vector pos;
		  pos[0] = box.x;
		  pos[2] = box.y;
		  int powerSourceFlags = ECE_CREATEPHYSICS || ECE_PLACE_ON_SURFACE;
		  auto obj = GetGame().CreateObjectEx("IE_FC_ElectricalBox", FCTeleportManager.SnapToGround(pos), powerSourceFlags);
		  vector powerSourceOrientation = "0 0 0";
		  powerSourceOrientation[0] = box.orientation;
		  obj.SetOrientation(powerSourceOrientation);
	  }
  }
	
	void FuelStationSoundUpdate( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
	    Param2<vector, bool> data;
	    if (ctx.Read(data)) {
			autoptr auto objects = new array<Object>;
			autoptr auto proxycargos = new array<CargoBase>;
			GetGame().GetObjectsAtPosition(data.param1, 0.1, objects, proxycargos);
			FuelStation station;
			foreach(auto object : objects) {
				station = FuelStation.Cast(object);
				if (station) {
					break;
				}
			}
			station.PlayFuelingSound(data.param2);
	    }
	}
}
