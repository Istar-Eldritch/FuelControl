// Script File

class FuelStationGroup {
	string name;
	vector position;
	float fuelCapacity;
	float fuelAmount;
	
	void FuelStationGroup(string _name, vector pos, float fuelCap, float fuel) {
		name = _name;
		position = pos;
		fuelCapacity = fuelCap;
		fuelAmount = fuel;
	}
	
	bool HasFuel() {
		return fuelAmount > 0 || fuelAmount == -1;
	}

	void RemoveFuel(float quantity) {
		if (fuelAmount != -1) {
			fuelAmount = fuelAmount - quantity;
			if (fuelAmount <  0) {
				fuelAmount = 0;
			}
		}
	}
}

class FuelStationManager {
	
	static int STATION_RADIOUS = 100;
	
	int lastRequestTime = 0;
	
	ref map<string, ref FuelStationGroup> stations = new ref map<string, ref FuelStationGroup>;
	
	void FuelStationManager() {
		if (GetGame().IsServer()) {
			FuelControlSettings config = GetFuelControlSettings();
			foreach(auto station: config.stations) {
				vector pos;
				pos[0] = station.x;
				pos[2] = station.y;
				stations[station.name] = new ref FuelStationGroup(station.name, pos, station.capacity * 1000, station.fuel * 1000);
			}
		}
	}
	
	ref FuelStationGroup FindStationForPump(vector pumpLocation) {
		foreach(auto station: stations){
			if (station && Math.IsPointInCircle(station.position, STATION_RADIOUS, pumpLocation)) {
				return station;
			}
		}
		
		return null;
	}
	
	void Save() {
		FuelControlSettings config = GetFuelControlSettings();
		foreach(auto cst: config.stations) {
			FuelStationGroup gst = stations[cst.name];
			cst.fuel = gst.fuelAmount / 1000;
		}
		config.Save();
	}
	
	void SendRequestStation(vector position) {
		auto currentTime = GetGame().GetTime();
		if (currentTime - lastRequestTime > 1000) {
			Print("Requestion station at position " + position);
			GetRPCManager().SendRPC("FuelControl", "RequestStation", new Param1<vector>(position), true);
			lastRequestTime = currentTime;
		}
	}
	
	void RequestStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<vector> data;
		if (GetGame().IsServer() && ctx.Read(data)) {
			vector position = data.param1;
			Print("[FuelControl] Client is requesting station for pump at " + position);
			auto station = FindStationForPump(position);
			if (station)
				GetRPCManager().SendRPC("FuelControl", "UpdateStation", new Param1<FuelStationGroup>(station), true, sender, target);
		}
	}
	
	void UpdateStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<FuelStationGroup> data;
		if (GetGame().IsClient() && ctx.Read(data)) {
			ref FuelStationGroup station = data.param1;
			Print("[FuelControl] Got update on station " + station.name + " from server");
			stations[station.name] = new ref FuelStationGroup(station.name, station.position, station.fuelCapacity, station.fuelAmount);
		}
	}
}

static ref FuelStationManager g_FuelStationManager;
static ref FuelStationManager GetFuelStationManager() {
    if (!g_FuelStationManager) {
        g_FuelStationManager = new ref FuelStationManager();
    }

    return g_FuelStationManager;
}