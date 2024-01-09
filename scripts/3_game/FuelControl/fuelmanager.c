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
		
	void FuelStationGroup(string _id, string _name, vector pos, float fuelCap, float fuel) {
		id = _id;
		name = _name;
		position = pos;
		fuelCapacity = fuelCap;
		fuelAmount = fuel;
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
	
	ref map<string, ref FuelStationGroup> stations = new ref map<string, ref FuelStationGroup>;
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
				stations[id] = new ref FuelStationGroup(id, station.name, pos, station.capacity * 1000, station.fuel * 1000);
			}
		}
		m_subscribers = new array<StationSubscriber>;
	}
	
	void Subscribe(StationSubscriber subscriber) {
		m_subscribers.Insert(subscriber);
	}
	
	void DeSubscribe(StationSubscriber subscriber) {
		m_subscribers.RemoveItem(subscriber);
	}
	
	ref FuelStationGroup FindStationForPump(vector pumpLocation) {
		foreach(auto station: stations){
			if (station && Math.IsPointInCircle(station.position, STATION_RADIOUS, pumpLocation)) {
				return station;
			}
		}
		
		return null;
	}

	ref FuelStationGroup FindStationByName(string name) {
		foreach(auto station: stations){
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
		foreach(auto id, auto st: stations) {
			StationConfig stc = new ref StationConfig(st.id, st.position[0], st.position[2], st.name, st.fuelCapacity / 1000, st.fuelAmount / 1000);
			config.stations.Insert(stc);
		}
		config.Save();
	}
	
	void SendRequestAllStations() {
		auto currentTime = GetGame().GetTime();
		if (currentTime - lastRequestTime > 1000) {
			Print("Requesting all stations");
			GetRPCManager().SendRPC("FuelControl", "RequestAllStations", null, true);
			lastRequestTime = currentTime;
		}
	}
	
	void SendRequestStation(vector position) {
		auto currentTime = GetGame().GetTime();
		if (currentTime - lastRequestTime > 1000) {
			Print("Requestion station at position " + position);
			GetRPCManager().SendRPC("FuelControl", "RequestStation", new Param1<vector>(position), true);
			lastRequestTime = currentTime;
		}
	}
	
	void RequestAllStations( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if (GetGame().IsServer()) {
			Print("[FuelControl] Client is requesting all stations");
			foreach (auto station: stations) {
				if (station) {
					GetRPCManager().SendRPC("FuelControl", "UpdateStation", new Param1<FuelStationGroup>(station), true, sender, target);
				}
			}
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
	
	void SendUpdateStation(FuelStationGroup station) {
		Print("Sending update on station " + station.name + " to server");
		stations[station.id] = new ref FuelStationGroup(station.id, station.name, station.position, station.fuelCapacity, station.fuelAmount);
		foreach (auto subscriber: m_subscribers) {
			if (subscriber) {
				subscriber.OnUpdate(stations[station.id]);
			}
		}
		GetRPCManager().SendRPC("FuelControl", "UpdateStation", new Param1<FuelStationGroup>(station), true);
	}
	
	void UpdateStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<FuelStationGroup> data;

		if (ctx.Read(data) && data.param1) {
			FuelStationGroup station = data.param1;			
			Print("[FuelControl] Got update on station " + station.name);
			stations[station.id] = new ref FuelStationGroup(station.id, station.name, station.position, station.fuelCapacity, station.fuelAmount);
			foreach (auto subscriber: m_subscribers) {
				if (subscriber) {
					subscriber.OnUpdate(stations[station.id]);
				}
			}
			Save();
		}
	}
	
	void SendDeleteStation(FuelStationGroup station) {
		stations.Remove(station.id);
		foreach (auto subscriber: m_subscribers) {
			if (subscriber) {
				subscriber.OnDelete(station);
			}
		}
		Print("Sending delete request for station " + station.name + " to server");
		GetRPCManager().SendRPC("FuelControl", "DeleteStation", new Param1<FuelStationGroup>(station), true);
	}

	void DeleteStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<FuelStationGroup> data;

		if (ctx.Read(data) && data.param1) {
			FuelStationGroup station = data.param1;			
			Print("[FuelControl] Got delete request for station " + station.name);
			stations.Remove(station.id);
			foreach (auto subscriber: m_subscribers) {
				if (subscriber) {
					subscriber.OnDelete(station);
				}
			}
		}
		
		Save();
	}

	void UpdateStats() {
		totalFuel = 0;
		totalCapacity = 0;

		foreach(auto station: stations) {
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
		foreach(auto station: stations) {
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
