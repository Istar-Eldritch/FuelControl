class CmdGetFuel extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "getfuel";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		auto stationName = cmd.positional.Get(2);

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
			Print("[FuelControl] Executing GetFuel chat command @" + station.name);
            string amount;
            if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amount = "" + scaledAmount + "L";
            } else {
                amount = "Infinite";
            }
            text = "Fuel available at " + station.name + ": " + amount;
        } else {
            text = "Could not find the fuel station";
        }
        auto parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
}