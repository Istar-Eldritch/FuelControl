class FCTeleportManager {
	int lastRequestTime = 0;

    void SendTeleportToStation(FuelStationGroup station) {
		auto currentTime = GetGame().GetTime();
		if (currentTime - lastRequestTime > 1000) {
			Print("[FuelControl] Requesting teleport to station " + station.name);
			GetRPCManager().SendRPC("IE_FC", "TeleportToStation", new Param1<FuelStationGroup>(station), true);
			lastRequestTime = currentTime;
		}
	}
	
	void TeleportToStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<FuelStationGroup> data;
		#ifdef JM_COT
		if (GetGame().IsServer() && ctx.Read(data)) {
			ref FuelStationGroup station = data.param1;
			Print("[FuelControl] Got teleport request to " + station.name + " from client " + sender.GetId());
			auto player = PlayerBase.Cast(sender.GetPlayer());
			player.SetLastPosition();
			player.SetWorldPosition(FCTeleportManager.SnapToGround(station.position));
		}
		#endif
	}
	
	static vector SnapToGround(vector pos) {
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY( pos_x, pos_z );
		vector tmp_pos = Vector( pos_x, pos_y, pos_z );
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}
}

static ref FCTeleportManager g_FCTeleportManager;
static ref FCTeleportManager FCGetTeleportManager() {
    if (!g_FCTeleportManager) {
      g_FCTeleportManager = new ref FCTeleportManager();
    }

    return g_FCTeleportManager;
}