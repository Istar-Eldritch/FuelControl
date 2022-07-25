
class CmdGetStations extends CmdHandler {
    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "getstations";
    }

    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {
		
		foreach(auto station: GetFuelStationManager().stations) {
			string amount;
			if (station.fuelAmount >= 0) {
                auto scaledAmount = station.fuelAmount / 1000;
                amount = "" + scaledAmount + "L";
            } else {
                amount = "Infinite";
            }
			string text = station.name + ": " + amount;
			auto parameter = new Param2<string, string>(text, "colorStatusChannel");
			GetRPCManager().SendRPC("FuelControl", "HandleChatMessage", parameter, true, sender);
		}
    }
}