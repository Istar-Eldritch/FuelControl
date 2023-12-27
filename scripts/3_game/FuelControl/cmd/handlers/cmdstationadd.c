class CmdStationAdd extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "station" && cmd.positional.Get(2) == "add";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
        Param2<string, string> parameter;
        if (cmd.positional.Count() == 3) {
            parameter = new Param2<string, string>("The name of the station must be provided", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }
		
		string stationName = cmd.positional.Get(3);
        float x;
        float y;

        string pos = cmd.mapped.Get("pos");
        if(pos && pos.Contains(",")) {
            TStringArray bits = new TStringArray;
            pos.Split(",", bits);
            x = bits.Get(0).ToFloat();
            y = bits.Get(1).ToFloat();
        } else {
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

        vector vpos;
		vpos[0] = x;
		vpos[2] = y;

        auto manager = GetFuelStationManager();
        ref FuelStationGroup station;
        // Check a station doesn't exist already with that name
        station = manager.FindStationByName(stationName);
        if (station) {
            parameter = new Param2<string, string>("A station with that name already exists", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }

        // Check a station doesn't exist already at that position
        station = GetFuelStationManager().FindStationForPump(vpos);
        if (station) {
            parameter = new Param2<string, string>("A station in this position already exists: " + station.name, "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }
		
		
		string id = FuelStationManager.GenId(stationName);
        station = new ref FuelStationGroup(id, stationName, vpos, -1 * 1000, -1 * 1000);

        manager.stations.Insert(id, station);
        manager.Save();
        parameter = new Param2<string, string>("Station " + station.name + " added", "colorStatusChannel");
        
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
}