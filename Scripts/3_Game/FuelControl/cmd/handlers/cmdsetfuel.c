class CmdSetFuel extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "setfuel";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
        Param2<string, string> parameter;
        if (cmd.positional.Count() == 2) {
            parameter = new Param2<string, string>("A valid amount of fuel in Liters must be provided", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
            return;
        }

        Print("[FuelControl] Executing SetFuel chat command - " + cmd.positional.Get(2) + " " + cmd.positional.Get(3));

		string stationName;
		float amount;
		if(cmd.positional.Count() == 4) {
			stationName = cmd.positional.Get(2);
			amount = cmd.positional.Get(3).ToFloat();
		} else {
			amount = cmd.positional.Get(2).ToFloat();
		}

        auto manager = GetFuelStationManager();
        ref FuelStationGroup station;

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
			Print("[FuelControl] Executing SetFuel chat command - " + station.name +  " = " +  amount + "L");
            station.fuelAmount = amount * 1000;
            manager.Save();
			string amountStr;
			if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amountStr = "" + scaledAmount + "L";
            } else {
                amountStr = "Infinite";
            }
            text = "Fuel available at " + station.name + ": " + amountStr;
        } else {
            text = "Could not find the fuel station";
        }
        parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
    }
}