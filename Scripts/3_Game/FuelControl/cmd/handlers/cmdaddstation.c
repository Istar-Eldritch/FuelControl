class CmdAddStation extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "addstation";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
        Param2<string, string> parameter;
        if (cmd.positional.Count() < 1) {
            parameter = new Param2<string, string>("The name of the station must be provided", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }
		
		string stationName;
        float x;
        float y;
		if(cmd.positional.Count() == 5) {
            x = cmd.positional.Get(2).ToFloat();
            y = cmd.positional.Get(3).ToFloat();
			stationName = cmd.positional.Get(4);
		} else {
            stationName = cmd.positional.Get(2);
        }

        ref FuelStationGroup station;

        if (!x || !y) {
            array<Man> players = new array<Man>;
            GetGame().GetWorld().GetPlayerList(players);
            foreach (auto player: players) {
                if (player.GetIdentity().GetId() == sender.GetId()) {
                    auto playerLocation = player.GetPosition();
                    x = playerLocation[0];
                    y = playerLocation[2];
                    break;
                }
            }
        }

		Print("[FuelControl] Executing AddStation chat command x:" + x + ",y:" + y + " name: " + stationName);

        vector pos;
		pos[0] = x;
		pos[2] = y;

        auto manager = GetFuelStationManager();

        // Check a station doesn't exist already with that name
        station = manager.FindStationByName(stationName);
        if (station) {
            parameter = new Param2<string, string>("A station with that name already exists", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }

        // Check a station doesn't exist already at that position
        station = GetFuelStationManager().FindStationForPump(pos);
        if (station) {
            parameter = new Param2<string, string>("A station in this position already exists: " + station.name, "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }

        station = new ref FuelStationGroup(stationName, pos, -1 * 1000, -1 * 1000);

        manager.stations.Insert(stationName, station);
        manager.Save();
        parameter = new Param2<string, string>("Station " + station.name + " added", "colorStatusChannel");
        
        
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
}