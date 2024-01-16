
modded class MissionBase {
  void MissionBase() {
    Print("[FuelControl] Creating mission base");
    FuelControlSettings settings = GetFuelControlSettings();
    GetRPCManager().AddRPC("IE_FC", "FuelControlSettingsOnSyncRPC", settings, SingleplayerExecutionType.Both);

    FuelStationManager manager = GetFuelStationManager();
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncStation", manager, SingleplayerExecutionType.Both);
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncAll", manager, SingleplayerExecutionType.Both);
    GetRPCManager().AddRPC("IE_FC", "FuelStationManagerSyncDeleteStation", manager, SingleplayerExecutionType.Both);

    CmdManager cmdManager = GetCmdManager();
    cmdManager.RegisterHandler(new ref CmdStationAdd());
    cmdManager.RegisterHandler(new ref CmdStationDel());
    cmdManager.RegisterHandler(new ref CmdStationInfo());
    cmdManager.RegisterHandler(new ref CmdStationEdit());
    cmdManager.RegisterHandler(new ref CmdStationList());

    cmdManager.RegisterHandler(new ref CmdStatus());
    cmdManager.RegisterHandler(new ref CmdSpawn());

    GetRPCManager().AddRPC("IE_FC", "HandleChatCommand", cmdManager, SingleplayerExecutionType.Both);
    GetRPCManager().AddRPC("IE_FC", "HandleChatMessage", cmdManager, SingleplayerExecutionType.Both);

    FCTeleportManager teleportManager = FCGetTeleportManager();
    GetRPCManager().AddRPC("IE_FC", "TeleportToStation", teleportManager, SingleplayerExecutionType.Both);

    if (GetGame().IsClient()) {
      settings.SyncSettings();
	  manager.SyncAll();
    }

    if (GetGame().IsServer()) {
      if(settings.settings.spawn_enabled) {
        GetFuelSpawner().Start();
      }
    }
  }
}
