class CmdStationInfo extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "station" && cmd.positional.Get(2) == "info";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		auto stationName = cmd.positional.Get(3);

        Param2<string, string> parameter;

        ref FuelStationGroup station;

        if (stationName) {
            station = GetFuelStationManager().FindStationByName(stationName);
        } else {
            array<Man> players = new array<Man>;
            GetGame().GetWorld().GetPlayerList(players);
            foreach (auto player: players) {
                if (player.GetIdentity().GetId() == sender.GetId()) {
                    auto playerLocation = player.GetPosition();
                    station = GetFuelStationManager().FindStationForPump(playerLocation);
                    break;
                }
            }
        }

        string text;
        if (station) {
			Print("[FuelControl] Executing station info for " + station.name);

            parameter = new Param2<string, string>("Name: " + station.name, "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);

            string amount;
            if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amount = "" + scaledAmount + "L";
            } else {
                amount = "Infinite";
            }
            parameter = new Param2<string, string>("Fuel Available: " + amount, "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);

            string capacity;
            if (station.fuelCapacity >= 0) {
                auto scaledCapacity = station.fuelCapacity / 1000;
                capacity = "" + scaledCapacity + "L";
            } else {
                capacity = "Infinite";
            }
            parameter = new Param2<string, string>("Fuel capacity: " + capacity, "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
        } else {
            parameter = new Param2<string, string>("Could not find the station", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
        }

    }
}