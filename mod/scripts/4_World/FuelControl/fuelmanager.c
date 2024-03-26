class StationSubscriber: Managed {

	void OnUpdate(FuelStationGroup station) {}
	
	void OnDelete(string id) {}
}

// Script File

class FuelStationGroup: Managed {
	ref IEFCStationConfig m_config;
	ref IEFCStationState m_state;
	
	void FuelStationGroup(IEFCStationConfig config, IEFCStationState state) {
		m_config = config;
		m_state = state;
	}
	
	void SetProp(string prop, string value) {
		TStringArray tokens = new TStringArray;
		int liquidId;
		string fuelType;
		float fuelV;
		prop.Split(".", tokens);
		// EnScript.SetClassVar(myClass, "m_Counter", 0, 5.0);
		switch(tokens[0]) {
			case "config":
				switch(tokens[1]) {
					case "id":
						break;
					case "fuels":
						liquidId = tokens[2].ToInt();
						fuelType = IE_FC_StringForLiquid(liquidId);
						fuelV = value.ToFloat();
						auto config = m_config.fuels.Get(fuelType);
						if (config) {
							config.capacity = fuelV;
						} else {
							m_config.fuels.Set(fuelType, new IEFCStationFuelConfig(fuelV));
						}
						break;
					case "name":
						m_config.name = value;
						break;
					case "x":
						m_config.x = value.ToFloat();
						break;
					case "y":
						m_config.y = value.ToFloat();
						break;
					default:
						CF_Log.Error("Invalid prop " + prop + " updating station. Likely a bug");
				}
				break;
			case "state":
				switch(tokens[1]) {
					case "fuels":
						liquidId = tokens[2].ToInt();
						fuelType = IE_FC_StringForLiquid(liquidId);
						fuelV = value.ToFloat();
						auto state = m_state.fuels.Get(fuelType);
						if (state) {
							state.quantity = fuelV;
						} else {
							m_state.fuels.Set(fuelType, new IEFCStationFuelState(fuelV));
						}
						break;
				}
				break:
			default:
				CF_Log.Error("Invalid prop " + prop + " updating station. Likely a bug");
				return;
		}
	}

	// Returns the amount of fuel in this station (in Liters)
	float GetFuel(int fuelType) {
		auto fuel = m_state.fuels.Get(IE_FC_StringForLiquid(fuelType));
		if (fuel) {
			return fuel.quantity;
		}
		return 0;
	}

	// Returns the total capacity in liters
	float GetCapacity(int fuelType) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_config.fuels.Get(fuelId);
		if (fuel) {
			return fuel.capacity;
		}
		return 0;
	}
	
	void SetCapacity(int fuelType, float capacity) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_config.fuels.Get(fuelId);
		if (fuel) {
			fuel.capacity = capacity;
			
		} else {
			fuel = new IEFCStationFuelConfig(capacity);
		}
		m_config.fuels.Set(fuelId, fuel);
		// TODO: Set dirty flag on fuel config
	}
	
	bool HasFuel(int fuelType) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_state.fuels.Get(fuelId);
		if (fuel) {
			return fuel.quantity > 0 || fuel.quantity == -1;
		}
		return false;
	}
	
	void SetFuel(int fuelType, float amount) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_state.fuels.Get(fuelId);
		if (fuel) {
			fuel.quantity = amount;
			
		} else {
			fuel = new IEFCStationFuelState(amount);
		}
		m_state.fuels.Set(fuelId, fuel);
		// TODO: Set dirty flag on fuel config.
	}

	// Return the available capacity in this station in Liters
	float AvailableCapacity(int fuelType) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_config.fuels.Get(fuelId);
		auto fuel_state = m_state.fuels.Get(fuelId);
		if (fuel) {
			if (fuel.capacity == -1) {
				return - 1;
			}

			if (fuel_state.quantity == -1) {
				return 0;
			}
		
			return fuel.capacity - fuel_state.quantity;
		}
		
		return 0;
	}

	void RemoveFuel(int fuelType, float amount) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_state.fuels.Get(fuelId);
		if (fuel) {
			if (fuel.quantity != -1) {
				fuel.quantity = fuel.quantity - amount;
				if (fuel.quantity <  0) {
					fuel.quantity = 0;
				}
				m_state.fuels.Set(fuelId, fuel);
			}

			auto source = GetPowerSource();
			if (source) {
				auto consumption = GetFuelControlSettings().settings.pump_consumption;
				source.GetCompEM().ConsumeEnergy(consumption * amount);
			}
		}
	}

	// Add the provided quantity of fuel (in Liters)
	void AddFuel(int fuelType, float amount) {
		string fuelId = IE_FC_StringForLiquid(fuelType);
		auto fuel = m_state.fuels.Get(fuelId);
		auto fuel_config = m_config.fuels.Get(fuelId);
		if (fuel) {
			if(fuel.quantity != -1) {
				fuel.quantity = fuel.quantity + amount;
				if (fuel_config.capacity != -1 && fuel.quantity > fuel_config.capacity) {
					fuel.quantity = fuel_config.capacity;
				}
			}
		} else {
			fuel = new IEFCStationFuelState(amount);
		}
		
		m_state.fuels.Set(fuelId, fuel);

	}
	
	bool HasEnergy(float amount = -1) {
		auto source = GetPowerSource();
		if (amount == -1) {
			amount = GetFuelControlSettings().settings.pump_consumption;
		}
		if (source) {
			return source.GetCompEM().CanWork(amount);
		}
		return false;
	}
	
	vector GetPosition() {
		vector position = "0 0 0";
		position[0] = m_config.x;
		position[2] = m_config.y;
		return position;
	}
	
	IE_FC_StationPowerSource GetPowerSource() {
		autoptr auto objects = new array<Object>;
		autoptr auto proxycargos = new array<CargoBase>;
		auto position = GetPosition();
		GetGame().GetObjectsAtPosition(position, FuelStationManager.STATION_RADIOUS, objects, proxycargos);
		foreach(auto object : objects) {
			IE_FC_StationPowerSource source = IE_FC_StationPowerSource.Cast(object);
			if (source) {
				return source;
			}
		}
		return null;
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
	int last_state_save = 0;
	int last_config_save = 0;
	
	ref map<string, ref FuelStationGroup> m_stations = new map<string, ref FuelStationGroup>;
	
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
			CF_Log.Trace("[FuelControl] Stations in config: " + config.stations_config.Count());
			for(int i = 0; i < config.stations_config.Count(); i++) {
				auto station_config = config.stations_config[i];
				auto station_state = config.stations_state[i];
				m_stations[station_config.id] = new FuelStationGroup(station_config, station_state);
			}
		}
	}
	
	void UpdateStation(string id, string prop, string value, bool sync = false) {
		FuelStationGroup station = m_stations.Get(id);
		if (station == null) {
			IEFCStationConfig config = new IEFCStationConfig(id, 0, 0, "", new map<string, ref IEFCStationFuelConfig>);
			IEFCStationState state = new IEFCStationState(id, new map<string, ref IEFCStationFuelState>);
			station = new FuelStationGroup(config, state);
			m_stations.Set(id, station);
		}
		station.SetProp(prop, value);

		if (GetGame().IsServer()) {
			SaveConfig();
			SaveState();
		}
		
		if (sync) {
			SyncStation(id, prop, value, sync);
		}
	}
	
	void DeleteStation(string id, bool sync = false) {
		m_stations.Remove(id);
		
		if (GetGame().IsServer()) {
			SaveConfig();
			SaveState();
		}
		if (sync) {
			SyncDeleteStation(id);
		}
	}
	
	FuelStationGroup FindStationForPump(vector pumpLocation) {
		foreach(auto station: m_stations){
			if (station && Math.IsPointInCircle(station.GetPosition(), STATION_RADIOUS, pumpLocation)) {
				return station;
			}
		}
		
		return null;
	}

	FuelStationGroup FindStationByName(string name) {
		foreach(auto station: m_stations){
			if (station && station.m_config.name == name) {
				return station;
			}
		}
		
		return null;
	}
	
	void SaveState() {
		int now = GetGame().GetTime();
		if (last_state_save <= now + 60000) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(_SaveState, 60000);
			last_state_save = now + 60000;
		}
	}
	
	void _SaveState() {
		FuelControlSettings config = GetFuelControlSettings();
		config.stations_state = new array<ref IEFCStationState>;
		foreach(auto id, auto st: m_stations) {
			config.stations_state.Insert(st.m_state);
		}
		config.SaveStationState();
	}

	void SaveConfig() {
		int now = GetGame().GetTime();
		if (last_config_save <= now + 60000) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(_SaveConfig, 60000);
			last_config_save = now + 60000;
		}
	}
	
	void _SaveConfig() {
		FuelControlSettings config = GetFuelControlSettings();
		config.stations_config = new array<ref IEFCStationConfig>;
		foreach(auto id, auto st: m_stations) {
			config.stations_config.Insert(st.m_config);
		}
		config.SaveStationConfig();
	}
	
	void SyncAll() {
		if (GetGame().IsClient()) {
			CF_Log.Debug("[FuelControl] Requesting update with all stations");
			GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncAll", null, true);	
		} else {
			CF_Log.Error("[FuelControl] Trying to request sync from server. This is likely a bug!");
		}
	}
	
	void FuelStationManagerSyncAll( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if (GetGame().IsServer()) {
			CF_Log.Debug("[FuelControl] Client is requesting all stations");
			foreach (auto id, auto station: m_stations) {
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.id", station.m_config.id, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.name", station.m_config.name, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.x", "" + station.m_config.x, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.y", "" + station.m_config.y, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.fuels." + LIQUID_GASOLINE + ".capacity", "" + station.m_config.fuels.Get(IE_FC_StringForLiquid(LIQUID_GASOLINE)).capacity, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.fuels." + LIQUID_DIESEL + ".capacity", "" + station.m_config.fuels.Get(IE_FC_StringForLiquid(LIQUID_DIESEL)).capacity, true), true, sender, target);	
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "config.fuels." + IE_FC_LIQUID_AVGAS + ".capacity", "" + station.m_config.fuels.Get(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS)).capacity, true), true, sender, target);	
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "state.fuels." + LIQUID_GASOLINE + ".available", "" + station.m_state.fuels.Get(IE_FC_StringForLiquid(LIQUID_GASOLINE)).quantity, true), true, sender, target);
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "state.fuels." + LIQUID_DIESEL + ".available", "" + station.m_state.fuels.Get(IE_FC_StringForLiquid(LIQUID_DIESEL)).quantity, true), true, sender, target);	
				GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, "state.fuels." + IE_FC_LIQUID_AVGAS + ".available", "" + station.m_state.fuels.Get(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS)).quantity, true), true, sender, target);
			}
		}
	}

	void SyncStation(string id, string prop, string value, bool push = false) {
		CF_Log.Trace("[FuelControl] Sending " + prop + " = " + value + " update on station " + id);
		GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncStation", new Param4<string, string, string, bool>(id, prop, value, push), true);
	}
	
	void FuelStationManagerSyncStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param4<string, string, string, bool> data;
		if (ctx.Read(data)) {
			string id = data.param1;
			string prop = data.param2;
			string value = data.param3;
			bool isUpdate = data.param4;
			if (isUpdate) {
				CF_Log.Trace("[FuelControl] Got update on station " + id + ": " + prop + " = " + value);
				UpdateStation(id, prop, value, GetGame().IsServer());
			} else {
				CF_Log.Error("[FuelControl] Got update request on station " + id + " but its not implemented");
			}
		} else {
			CF_Log.Error("[FuelControl] We can't sync a station if there is no information about a station. Likely a bug");
		}
	}
	
	void SyncDeleteStation(string id) {
		CF_Log.Debug("[FuelControl] Sending delete request for station " + id);
		GetRPCManager().SendRPC("IE_FC", "FuelStationManagerSyncDeleteStation", new Param1<string>(id), true);
	}

	void FuelStationManagerSyncDeleteStation( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<string> data;

		if (ctx.Read(data)) {
			CF_Log.Debug("[FuelControl] Got delete request for station " + data.param1);
			DeleteStation(data.param1, GetGame().IsServer());
		}
	}

	// Spawn the provided fuel amount (in liters)
	void Spawn(int fuelType, float amount) {
		amount = Math.Floor(amount);
		CF_Log.Debug("[FuelControl] Spawning " + amount + "L of " + IE_FC_StringForLiquid(fuelType));

		array<ref FuelStationGroup> filteredStations = new array<ref FuelStationGroup>;
		foreach(auto station: m_stations) {
			float availableCapacity = station.AvailableCapacity(fuelType);
			if (availableCapacity == -1 || availableCapacity > 0) {
				filteredStations.Insert(station);
			}
		}

		float minToAdd = GetFuelControlSettings().settings.spawn_minimum;
		while(amount > 0 && filteredStations.Count() > 0) {
			int idx = Math.RandomInt(0, filteredStations.Count());
			auto sta = filteredStations.Get(idx);
			auto capacity = sta.AvailableCapacity(fuelType);
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
			sta.AddFuel(fuelType, toAdd);
			CF_Log.Trace("[FuelControl] Added " + toAdd + "L to " + sta.m_config.name);
			amount = amount - toAdd;
			if (capacity != -1) {
				capacity = capacity - toAdd;
				if(capacity == 0) {
					filteredStations.Remove(idx);
				}
			}
		}
		SaveState();
	}
}

static ref FuelStationManager g_FuelStationManager;
static FuelStationManager GetFuelStationManager() {
    if (!g_FuelStationManager) {
        g_FuelStationManager = new FuelStationManager();
    }

    return g_FuelStationManager;
}
