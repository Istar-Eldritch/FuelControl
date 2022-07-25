class CmdDelStation extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "delstation";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		auto stationName = cmd.positional.Get(2);

        ref FuelStationGroup station;

        auto manager = GetFuelStationManager();

        if (stationName) {
            station = manager.FindStationByName(stationName);
        } else {
            array<Man> players = new array<Man>;
            GetGame().GetWorld().GetPlayerList(players);
            foreach (auto player: players) {
                if (player.GetIdentity().GetId() == sender.GetId()) {
                    auto playerLocation = player.GetPosition();
                    station = manager.FindStationForPump(playerLocation);
                    break;
                }
            }
        }

        string text;
        if (station) {
			Print("[FuelControl] Executing DelStation chat command for " + station.name);
            manager.stations.Remove(station.name);
            manager.Save();
            text = "Station " + station.name + " deleted";
        } else {
            text = "Could not find the fuel station";
        }
        auto parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
}