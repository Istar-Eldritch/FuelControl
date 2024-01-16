class CmdStationList extends CmdHandler {

    override bool CanHandle(ref CmdArgs cmd) {
        return cmd.positional.Get(0) == "fc" && cmd.positional.Get(1) == "station" && cmd.positional.Get(2) == "list";
    }
    
    override void HandleCmd(ref CmdArgs cmd, ref PlayerIdentity sender) {

        string stationName = cmd.positional.Get(3);
        Param2<string, string> parameter;

        float minFuel = 0;
        string sMinFuel = cmd.mapped.Get("min-fuel");
        if (sMinFuel) {
            minFuel = sMinFuel.ToFloat();
        }

        float maxFuel = -1;
        string sMaxFuel = cmd.mapped.Get("max-fuel");
        if (sMaxFuel) {
            maxFuel = sMaxFuel.ToFloat();
        }

        foreach(auto station: GetFuelStationManager().m_stations) {
            float fuel = station.GetFuel();
            bool hasMinFuel = fuel == -1 || fuel >= minFuel;
            bool hasMaxFuel = maxFuel == -1 || fuel <= maxFuel;
            bool matchesName = true;
			if(stationName) {
				matchesName = station.name.Contains(stationName);
			}
            if (hasMinFuel && hasMaxFuel && matchesName) {
                parameter = new Param2<string, string>(station.name + ": " + fuel + "L", "colorStatusChannel");
                GetRPCManager().SendRPC("IE_FC", "HandleChatMessage", parameter, true, sender);
            }
        }
    }
}