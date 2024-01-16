class CmdStatus extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "status";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		Param2<string, string> parameter;
        auto manager = GetFuelStationManager();
		manager.UpdateStats();
		
        string fuel;
        if (manager.totalFuel >= 0) {
            fuel = "" + manager.totalFuel + "L";
        } else {
            fuel = "Infinite";
        }
        parameter = new Param2<string, string>("Total fuel available: " + fuel, "colorStatusChannel");
        GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);

        string capacity;
        if (manager.totalCapacity >= 0) {
            capacity = "" + manager.totalCapacity + "L";
        } else {
            capacity = "Infinite";
        }
        parameter = new Param2<string, string>("Total capacity available: " + capacity, "colorStatusChannel");
        GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);

        if (manager.totalFuel >= 0 && manager.totalCapacity > 0) {
			float percentage = manager.totalFuel / manager.totalCapacity * 100;
            parameter = new Param2<string, string>("Reserves are at " + Math.Round(percentage) + "%", "colorStatusChannel");
            GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);
        }
    }
}