class CmdStationEdit extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "station" && cmd.positional.Get(2) == "edit";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
        Param2<string, string> parameter;

		auto currentName = cmd.positional.Get(3);

        auto sfuel = cmd.mapped.Get("fuel");
        float fuel;
        if (sfuel) {
            fuel = sfuel.ToFloat();
        }

        auto scap = cmd.mapped.Get("capacity");
        float capacity;
        if (scap) {
            capacity = scap.ToFloat();
        }

        string newName = cmd.mapped.Get("name");

        CF_Log.Debug("[FuelControl] Executing station edit - " + currentName + " " + newName + " " + fuel + " " + capacity );

        auto manager = GetFuelStationManager();
        ref FuelStationGroup station;

        if (currentName) {
            station = manager.FindStationByName(currentName);
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
            if (sfuel) {
                station.fuelAmount = fuel * 1000;
            }
            if (scap) {
                station.fuelCapacity = capacity * 1000;
            }

            if (newName) {
                station.name = newName;
            }

            manager.Save();
			text = "Station " + station.name + " updated";
        } else {
            text = "Could not find the fuel station";
        }
        parameter = new Param2<string, string>(text, "colorStatusChannel");
        GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);
    }
}