class CmdStatus extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "status";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		Param2<string, string> parameter;
        auto manager = GetFuelStationManager();

        float totalFuel = 0;
        float totalCapacity = 0;

        foreach(auto station: manager.stations) {
            if (totalFuel != -1) {
                if (station.fuelAmount >= 0) {
                    totalFuel = totalFuel + station.fuelAmount;
                } else {
                    totalFuel = -1;
                }
            }

            if (totalCapacity != -1) {
                if (station.fuelCapacity >= 0) {
                    totalCapacity = totalCapacity + station.fuelCapacity;
                } else {
                    totalCapacity = -1;
                }
            }
        }

        string fuel;
        if (totalFuel >= 0) {
            auto scaledFuel = totalFuel / 1000;
            fuel = "" + scaledFuel + "L";
        } else {
            fuel = "Infinite";
        }
        parameter = new Param2<string, string>("Total fuel available: " + fuel, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);

        string capacity;
        if (totalCapacity >= 0) {
            auto scaledCapacity = totalCapacity / 1000;
            capacity = "" + scaledCapacity + "L";
        } else {
            capacity = "Infinite";
        }
        parameter = new Param2<string, string>("Total capacity available: " + capacity, "colorStatusChannel");
        GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);

        if (totalFuel >= 0 && totalCapacity > 0) {
			float percentage = totalFuel / totalCapacity * 100;
            parameter = new Param2<string, string>("Reserves are at " + Math.Round(percentage) + "%", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
        }
    }
}