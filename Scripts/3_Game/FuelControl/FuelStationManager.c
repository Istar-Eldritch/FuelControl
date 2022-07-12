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
}

class FuelStationManager {
	
	static int STATION_RADIOUS = 45;
	
	private ref map<string, ref FuelStationGroup> stations;
	
	void FuelStationManager() {
		stations = new map<string, ref FuelStationGroup>();
		if(GetGame().IsServer()) {
			FuelControlSettings config = GetFuelControlSettings();
			foreach(auto station: config.stations) {
				vector pos;
				pos[0] = station.x;
				pos[2] = station.y;
				stations[station.name] = new ref FuelStationGroup(station.name, pos, station.capacity, station.fuel);
			}
			// TODO: Load this from a save file if it exists.
			
		} else {
			stations = new map<string, ref FuelStationGroup>();
		}
	}
	
	ref FuelStationGroup FindStationForPump(vector pumpLocation) {
		Print("[FuelControl] Check fuel pump at " + pumpLocation);
		foreach(auto station: stations){
			Print("[FuelControl] Checking station " + station);
			if (station && Math.IsPointInCircle(station.position, STATION_RADIOUS, pumpLocation)) {
				Print("[FuelControl] Fuel pump at " + pumpLocation + " is part of the " + station.name + " station");
				return station;
			}
		}
		
		return null;
	}

	void SyncFuelStations() {
		Print("[FuelControl] Requesting fuel stations from server");
		GetRPCManager().SendRPC("FuelControl", "GetFuelStations", null, true);
	}
	
	void GetFuelStations( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<map<string, ref FuelStationGroup>> data;
		if (ctx.Read(data)) {
			Print("[FuelControl] Got fuel stations from server");
			ref map<string, ref FuelStationGroup> sts = data.param1;
			foreach(auto station: sts) {
				Print("[FuelControl] Station " + station.name + " @ " + station.position);
				stations[station.name] = new ref FuelStationGroup(station.name, station.position, station.fuelCapacity, station.fuelAmount);
			}
		} else if (GetGame().IsServer()) {
			// If the sender is not sending an update, then send all the station information back to it.
			GetRPCManager().SendRPC("FuelControl", "GetFuelStations", new Param1<map<string, ref FuelStationGroup>>(stations), true, sender, target);
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