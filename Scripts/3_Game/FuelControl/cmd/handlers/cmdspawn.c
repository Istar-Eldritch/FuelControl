class CmdSpawn extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "spawn";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		Param2<string, string> parameter;
        auto manager = GetFuelStationManager();

        float amount;
        string samount = cmd.positional.Get(2);
        if(samount) {
            amount = samount.ToFloat();
            manager.Spawn(amount);
            parameter = new Param2<string, string>("Done", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
        } else {
            parameter = new Param2<string, string>("The amount of fuel to be spawned must be provided", "colorStatusChannel");
            GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
        }
		
    }
}