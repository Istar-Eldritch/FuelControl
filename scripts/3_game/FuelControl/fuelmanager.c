class StationSubscriber {

	void OnUpdate(FuelStationGroup station) {}
	
	void OnDelete(FuelStationGroup station) {}
}

// Script File

class FuelStationGroup {
	string id;
	string name;
	vector position;
	// capacity in ml
	float fuelCapacity;
	// fuel amount in ml
	float fuelAmount;
	bool m_hasPower;
		
	void FuelStationGroup(string _id, string _name, vector pos, float fuelCap, float fuel) {
		id = _id;
		name = _name;
		position = pos;
		fuelCapacity = fuelCap;
		fuelAmount = fuel;
		m_hasPower = false;
	}

	// Returns the amount of fuel in this station (in Liters)
	float GetFuel() {
		return fuelAmount / 1000;
	}

	// Returns the total capacity in liters
	float GetCapacity() {
		return fuelCapacity / 1000;
	}
	
	bool HasFuel() {
		return fuelAmount > 0 || (fuelAmount / 1000) == -1;
	}

	// Return the available capacity in this station in Liters
	float AvailableCapacity() {
		if (fuelCapacity == -1000) {
			return -1;
		}

		if (fuelAmount == -1000) {
			return 0;
		}

		return (fuelCapacity - fuelAmount) / 1000;
	}

	void RemoveFuel(float quantity) {
		if ((fuelAmount / 1000) != -1) {
			fuelAmount = fuelAmount - quantity;
			if (fuelAmount <  0) {
				fuelAmount = 0;
			}
		}
	}

	// Add the provided quantity of fuel (in Liters)
	void AddFuel(float quantity) {
		if((fuelAmount / 1000) != -1) {
			fuelAmount = fuelAmount + quantity * 1000;
			if (fuelCapacity != -1000 && fuelAmount > fuelCapacity) {
				fuelAmount = fuelCapacity;
			}
		}
	}
	
	bool HasEnergy() {
		return m_hasPower;
	}

	void SetHasEnergy(bool hasEnergy) {
		m_hasPower = hasEnergy;
	}
	
}

class FuelStationManager {
	
	static int STATION_RADIOUS = 100;
	
	int lastRequestTime = 0;

	// total fuel in liters
	float totalFuel;
	// total capacity in liters
	float totalCapacity;
	string fullestStation;
	float fullestStationFreeCapacity;
	int last_save = 0;
	
	ref map<string, ref FuelStationGroup> m_stations = new ref map<string, ref FuelStationGroup>;
	ref array<StationSubscriber> m_subscribers;
	
	static string GenId(string seed) {
		CF_StringStream stream = new CF_StringStream(seed);
		CF_TextReader reader = new CF_TextReader(stream);
		CF_Base16Stream output = new CF_Base16Stream();
		CF_SHA256.Process(reader, output);
		string id = output.Encode();
		reader.Close();
		
		return id;
	}

	void FuelStationManager() {
		Print("StationManager constructor");
		if (GetGame().IsServer()) {
			FuelControlSettings config = GetFuelControlSettings();
			foreach(auto station: config.stations) {
				vector pos;
				pos[0] = station.x;
				pos[2] = station.y;
				string id;
				if (station.id) {
					id = station.id;
				} else {
					id = FuelStationManager.GenId(station.name);
				}
				m_stations[id] = new FuelStationGroup(id, station.name, pos, station.capacity * 1000, station.fuel * 1000);
			}
		}
		m_subscribers = new array<StationSubscriber>;
	}
	
	void UpdateStation(FuelStationGroup station, bool sync = false) {
		m_stations.Set(station.id, station);
		foreach (auto subscriber: m_subscribers) {
			if (subscriber) {
				subscriber.OnUpdate(station);
			}
		}
		
		if (GetGame().IsServer()) {
			Save();
		}
		if (sync) {
			SyncStation(station, true);
		}
	}
	
	void DeleteStation(FuelStationGroup station, bool sync = false) {
		m_stations.Remove(station.id);
		foreach (auto subscriber: m_subscribers) {
			if (subscriber) {
				subscriber.OnDelete(station);
			}
		}
		if (GetGame().IsServer()) {
			Save();
		}
		if (sync) {
			SyncDeleteStation(station);
		}
	}
	
	void Subscribe(StationSubscriber subscriber) {
		m_subscribers.Insert(subscriber);
	}
	
	void DeSubscribe(StationSubscriber subscriber) {
		m_subscribers.RemoveItem(subscriber);
	}
	
	FuelStationGroup FindStationForPump(vector pumpLocation) {
		foreach(auto station: m_stations){
			if (station && Math.IsPointInCircle(station.position, STATION_RADIOUS, pumpLocation)) {
				return station;
			}
		}
		
		return null;
	}

	FuelStationGroup FindStationByName(string name) {
		foreach(auto station: m_stations){
			if (station && station.name == name) {
				return station;
			}
		}
		
		return null;
	}
	
	void Save() {
		int now = GetGame().GetTime();
		if (last_save <= now + 1000) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(_Save, 1000);
			last_save = now + 1000;
		}
	}
	
	void _Save() {
		FuelControlSettings config = GetFuelControlSettings();
		config.stations = new ref array<ref StationConfig>;
		foreach(auto id, auto st: m_stations) {
			StationConfig stc = new ref StationConfig(st.id, st.position[0], st.position[2], st.name, st.fuelCapacity / 1000, st.fuelAmount / 1000);
			config.stations.Insert(stc);
		}
		config.Save();
	}
	
	void SyncAll(bool push = false) {
		if (push) {
			Print("[FuelControl] Sending update with all stations");
			JsonSerializer ser = new JsonSerializer;
			string data;
			ser.WriteToString(m_stations.GetValueArray(), false, data);
			GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncAll", new Param1<string>(data), true);
		} else {
			Print("[FuelControl] Requesting update with all stations");
			GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncAll", null, true);	
		}
	}
	
	void FuelStationManagerSyncAll( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		JsonSerializer ser = new JsonSerializer;
		Param1<string> data;
		if (ctx.Read(data)) {
			Print("[FuelControl] Got update with all stations");
			string error;
			array<FuelStationGroup> stations = new array<FuelStationGroup>;
			ser.ReadFromString(stations, data.param1, error);
			if (error) {
				Print("ERROR: " + error);
			}
			m_stations = new map<string, ref FuelStationGroup>;
			foreach (auto station: stations) {
				m_stations.Set(station.id, station);
			}
			if (GetGame().IsServer()) {
				Save();
				SyncAll(true);
			}
		} else if (GetGame().IsServer()) {
			Print("[FuelControl] Client is requesting all stations");
			string dat;
			ser.WriteToString(m_stations.GetValueArray(), false, dat);
			GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncAll", new Param1<string>(dat), true, sender, target);	
		}
	}
	
	void SyncStation(FuelStationGroup station, bool push = false) {
		Print("[FuelControl] Sending update on station " + station.name);
		JsonSerializer ser = new JsonSerializer;
		string data;
		ser.WriteToString(station, false, data);
		GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param2<string, bool>(data, push), true);
	}
	
	void FuelStationManagerSyncStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param2<string, bool> data;
		if (ctx.Read(data)) {
			string error;
			FuelStationGroup station;
			JsonSerializer ser = new JsonSerializer;
			ser.ReadFromString(station, data.param1, error);
			bool isUpdate = data.param2;
			if (isUpdate) {
				Print("[FuelControl] Got update on station " + station.name);
				UpdateStation(station, GetGame().IsServer());
			} else {
				Print("[FuelControl] Got update request on station " + station.name);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param2<string, bool>(data.param1, true), true, sender, target);
			}
		} else {
			Print("[FuelControl] We can't sync a station if there is no information about a station. Likely a bug");
		}
	}
	
	void SyncDeleteStation(FuelStationGroup station) {
		Print("[FuelControl] Sending delete request for station " + station.name);
		JsonSerializer ser = new JsonSerializer;
		string data;
		ser.WriteToString(station, false, data);
		GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncDeleteStation", new Param1<string>(data), true);
	}

	void FuelStationManagerSyncDeleteStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<string> data;

		if (ctx.Read(data)) {
			FuelStationGroup station = new FuelStationGroup("", "", "0 0 0", 0, 0);
			JsonSerializer ser = new JsonSerializer;
			string error;
			ser.ReadFromString(station, data.param1, error);
			Print("[FuelControl] Got delete request for station " + station.name);
			DeleteStation(station, GetGame().IsServer());
		}
		
		Save();
	}

	void UpdateStats() {
		totalFuel = 0;
		totalCapacity = 0;

		foreach(auto station: m_stations) {
            if (totalFuel != -1) {
                if (station.GetFuel() >= 0) {
                    totalFuel = totalFuel + station.GetFuel();
                } else {
                    totalFuel = -1;
                }
            }

            if (totalCapacity != -1) {
                if (station.GetCapacity() >= 0) {
                    totalCapacity = totalCapacity + station.GetCapacity();
                } else {
                    totalCapacity = -1;
                }
            }
        }
	}

	// Spawn the provided fuel amount (in liters)
	void Spawn(float amount) {
		amount = Math.Floor(amount);
		Print("[FuelControl] Spawning " + amount + "L of fuel");
		// UpdateStats();
		// float free = totalCapacity - totalFuel;
		// if (free < amount) {
		// 	foreach(auto st: stations) {
		// 		st.fuelAmount = st.fuelCapacity;
		// 	}
		// 	Save();
		// 	return;
		// }

		ref array<ref FuelStationGroup> filteredStations = new ref array<ref FuelStationGroup>;
		foreach(auto station: m_stations) {
			float availableCapacity = station.AvailableCapacity();
			if (availableCapacity == -1 || availableCapacity > 0) {
				filteredStations.Insert(station);
			}
		}

		float minToAdd = GetFuelControlSettings().settings.spawn_minimum;
		while(amount > 0 && filteredStations.Count() > 0) {
			int idx = Math.RandomInt(0, filteredStations.Count());
			auto sta = filteredStations.Get(idx);
			auto capacity = sta.AvailableCapacity();
			float toAdd;
			if (capacity == -1) {
				if(amount <= minToAdd) {
					toAdd = amount;
				} else {
					toAdd = Math.RandomIntInclusive(minToAdd, amount);
				}
			} else {
				if (capacity <= minToAdd) {
					toAdd = capacity;
				} else {
					if(amount <= minToAdd) {
						toAdd = amount;
					} else {
						float max;
						if (amount < capacity) {
							max = amount;
						} else {
							max = capacity;
						}
						toAdd = Math.RandomIntInclusive(minToAdd, max);
					}
				}
			}
			sta.AddFuel(toAdd);
			Print("[FuelControl] Added " + toAdd + "L to " + sta.name);
			amount = amount - toAdd;
			if (capacity != -1) {
				capacity = capacity - toAdd;
				if(capacity == 0) {
					filteredStations.Remove(idx);
				}
			}
		}
		Save();
	}
}

static ref FuelStationManager g_FuelStationManager;
static ref FuelStationManager GetFuelStationManager() {
    if (!g_FuelStationManager) {
        g_FuelStationManager = new ref FuelStationManager();
    }

    return g_FuelStationManager;
}
