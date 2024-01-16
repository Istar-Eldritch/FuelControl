class CmdStationDel extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "station" && cmd.positional.Get(2) == "del";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		auto stationName = cmd.positional.Get(3);

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
			Print("[FuelControl] Executing station del for " + station.name);
			manager.DeleteStation(station, true);
            text = "Station " + station.name + " deleted";
        } else {
            text = "Could not find the station";
        }
        auto parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);
    }
}