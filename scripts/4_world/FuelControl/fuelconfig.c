class IEFCStationFuelState {
  float quantity;
	
	void IEFCStationFuelState(float _amount) {
		quantity = _amount;
	}

}

class IEFCStationState: Managed {
  string id;
  ref map<string, ref IEFCStationFuelState> fuels;
	
	void IEFCStationState(string _id, map<string, ref IEFCStationFuelState> _fuels) {
		id = _id;
		fuels = _fuels;
	}
}

class IEFCStationFuelConfig {
  float capacity;
	
	void IEFCStationFuelConfig( float _capacity) {
		capacity = _capacity;
	}
}

class IEFCStationConfig: Managed {
  string id;
  float x;
  float y;
  string name;
  ref map<string, ref IEFCStationFuelConfig> fuels;

  void IEFCStationConfig(string _id, float xx, float yy, string _name, ref map<string, ref IEFCStationFuelConfig> _fuels) {
	 id = _id;
    x = xx;
    y = yy;
    name = _name;
	fuels = _fuels;
  }
}

class LegacyStationConfig {
  string id;
  float x;
  float y;
  float orientation;
  string name;
  float fuel = -1; // If negative then there is no limit on this station
  float capacity = -1;

  void LegacyStationConfig(string _id, float xx, float yy, string _name, float _fuel, float _capacity) {
	  id = _id;
    x = xx;
    y = yy;
    name = _name;
    fuel = _fuel;
    capacity = _capacity;
  }
}

class IE_FC_PowerBoxConfig {
  string id;
  float x;
  float y;
  float orientation = 0;
  string name;

  void IE_FC_PowerBoxConfig(string _id, float xx, float yy, float _orientation, string _name) {
	id = _id;
    x = xx;
    y = yy;
	orientation = _orientation;
    name = _name;
  }
}

class IE_FC_VehicleConfig {
	float autonomy;
	string fuel_type;
	
	void IE_FC_VehicleConfig(float _autonomy, string _fuel_type) {
		autonomy = _autonomy;
		fuel_type = _fuel_type;
	}
}

class FCSettings {
  int spawn_enabled = true;
  int spawn_interval = 1800;
  int spawn_amount = 1;
  int spawn_minimum = 10;
  bool pump_car_refueling = true;
  bool pump_barrel_refueling = true;
  bool siphoning = true;
  float siphoning_limit = 98;
  bool default_pumps_have_fuel = 0;
  bool pumps_get_ruined = 1;
  bool measure_fuel_with_stick = 1;
  bool pumps_require_electricity = 0;
  int pump_consumption = 2;
}


string IE_FC_StringForLiquid(int liquidType) {
	switch (liquidType) {
		case LIQUID_GASOLINE:
			return "GASOLINE";
		case LIQUID_DIESEL:
			return "DIESEL";
		case IE_FC_LIQUID_AVGAS:
			return "AVGAS";
		default:
			return "ERROR";
	}
	return "ERROR";
}

int IE_FC_LiquidFromString(string liquidName) {
	switch (liquidName) {
		case "GASOLINE":
			return LIQUID_GASOLINE;
		case "DIESEL":
			return LIQUID_DIESEL;
		case "AVGAS":
			return IE_FC_LIQUID_AVGAS;
		default:
			return -1;
	}
	return -1;
}

class FuelControlSettings {
  
  static private const string DIR_PATH = "$profile:FuelControl";

  static private const string SETTINGS_PATH = DIR_PATH + "\\settings.json";
  static private const string LEGACY_STATIONS_PATH = DIR_PATH + "\\stations.json";
  static private const string STATIONS_CONFIG_PATH = DIR_PATH + "\\stations_config.json";
  static private const string STATIONS_STATE_PATH = DIR_PATH + "\\stations_state.json";
  static private const string POWER_BOX_PATH = DIR_PATH + "\\power_boxes.json";
  static private const string VEHICLE_CONFIG_PATH = DIR_PATH + "\\vehicle_config.json";
  static private const string LEGACY_VEHICLE_AUTONOMY_PATH = DIR_PATH + "\\vehicle_autonomy.json";
  static private const string LIQUID_TRANSFER_RATES_PATH = DIR_PATH + "\\liquid_transfer_rates.json";
  
  ref FCSettings settings = new ref FCSettings();
  ref array<ref IEFCStationConfig> stations_config = new ref array<ref IEFCStationConfig>;
  ref array<ref IEFCStationState> stations_state = new array<ref IEFCStationState>;
  ref array<ref IE_FC_PowerBoxConfig> power_boxes = new ref array<ref IE_FC_PowerBoxConfig>;
  ref map<string, ref IE_FC_VehicleConfig> vehicle_config = new ref map<string, ref IE_FC_VehicleConfig>;
  ref map<string, float> liquid_transfer_rates = new ref map<string, float>;
  
  void Load() {
    if (!FileExist(DIR_PATH)){
      MakeDirectory(DIR_PATH);
    }

    if (FileExist(SETTINGS_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading configuration");
      JsonFileLoader<FCSettings>.JsonLoadFile(SETTINGS_PATH, settings );
    }

    if (FileExist(STATIONS_STATE_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading stations state");
      JsonFileLoader<array<ref IEFCStationState>>.JsonLoadFile(STATIONS_STATE_PATH, stations_state );
    }
		
    if (FileExist(STATIONS_CONFIG_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading station configuration");
      JsonFileLoader<array<ref IEFCStationConfig>>.JsonLoadFile(STATIONS_CONFIG_PATH, stations_config );
	  string gasoline = IE_FC_StringForLiquid(LIQUID_GASOLINE);
	  string diesel = IE_FC_StringForLiquid(LIQUID_DIESEL);
	  string avgas = IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS);
	  foreach (auto config: stations_config) {
		auto gasoline_config = config.fuels.Get(gasoline);
		if (!gasoline_config) {
			config.fuels.Set(gasoline, new IEFCStationFuelConfig(-1));
		}
		auto diesel_config = config.fuels.Get(diesel);
		if (!diesel_config) {
			config.fuels.Set(diesel, new IEFCStationFuelConfig(-1));
		}
		auto avgas_config = config.fuels.Get(avgas);
		if (!avgas_config) {
			config.fuels.Set(avgas, new IEFCStationFuelConfig(-1));
		}

		ref IEFCStationState found;
		foreach(auto state: stations_state) {
			if (config.id == state.id) {
				found = state;
			}
		}

		if (!found) {
			auto fuelStates = new map<string, ref IEFCStationFuelState>;

			float gasoline_capacity = config.fuels.Get(gasoline).capacity;
			fuelStates.Set(gasoline, new IEFCStationFuelState(gasoline_capacity));
			
			float diesel_capacity = config.fuels.Get(diesel).capacity;
			fuelStates.Set(diesel, new IEFCStationFuelState(diesel_capacity));

			float avgas_capacity = config.fuels.Get(avgas).capacity;
			fuelStates.Set(avgas, new IEFCStationFuelState(avgas_capacity));

			stations_state.Insert(new IEFCStationState(config.id, fuelStates));
		} else {
			auto gasoline_state = found.fuels.Get(gasoline);
			if (!gasoline_state) {
				found.fuels.Set(gasoline, new IEFCStationFuelState(config.fuels.Get(gasoline).capacity));
			}
			auto diesel_state = found.fuels.Get(diesel);
			if (!diesel_state) {
				found.fuels.Set(diesel, new IEFCStationFuelState(config.fuels.Get(diesel).capacity));
			}
			auto avgas_state = found.fuels.Get(avgas);
			if (!avgas_state) {
				found.fuels.Set(avgas, new IEFCStationFuelState(config.fuels.Get(avgas).capacity));
			}
		}
	  }
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
			stations_state = new array<ref IEFCStationState>;
      	if (FileExist(LEGACY_STATIONS_PATH)){ //If config exist load File
      		MigrateLegacyStations();
    	} else {
			DefaultStations();
		}
    }
		
	if (FileExist(POWER_BOX_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading power box configuration");
      JsonFileLoader<array<ref IE_FC_PowerBoxConfig>>.JsonLoadFile(POWER_BOX_PATH, power_boxes );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultPowerBoxes();
    }
		
	if (FileExist(VEHICLE_CONFIG_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading vehicle configuration");
      JsonFileLoader<map<string, ref IE_FC_VehicleConfig>>.JsonLoadFile(VEHICLE_CONFIG_PATH, vehicle_config );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
		vehicle_config = new map<string, ref IE_FC_VehicleConfig>;
      	if (FileExist(LEGACY_VEHICLE_AUTONOMY_PATH)){ //If config exist load File
      		MigrateLegacyVehicleAutonomy();
    	} else {
			DefaultVehicleConfig();
		}
    }
		
	if (FileExist(LIQUID_TRANSFER_RATES_PATH)){ //If config exist load File
      CF_Log.Info("[FuelControl] Loading liquid transfer rates configuration");
      JsonFileLoader<map<string, float>>.JsonLoadFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultLiquidTransferRates();
    }
  }
  
  void Save() {
	if (GetGame().IsServer()) {
	    SaveStationConfig();
		SaveStationState();
		SaveSettings();
		SaveVehicleConfig();
		SaveLiquidTransfer();
		SavePowerBoxes();
	}
  }
	
  void SaveStationConfig() {
	JsonFileLoader<array<ref IEFCStationConfig>>.JsonSaveFile(STATIONS_CONFIG_PATH, stations_config );
  }
	
  void SaveStationState() {
	JsonFileLoader<array<ref IEFCStationState>>.JsonSaveFile(STATIONS_STATE_PATH, stations_state );
  }
	
  void SaveSettings() {
	JsonFileLoader<FCSettings>.JsonSaveFile(SETTINGS_PATH, settings);
  }

  void SaveVehicleConfig() {
	JsonFileLoader<map<string, ref IE_FC_VehicleConfig>>.JsonSaveFile(VEHICLE_CONFIG_PATH, vehicle_config );
  }
	
  void SaveLiquidTransfer() {
	JsonFileLoader<map<string, float>>.JsonSaveFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates);
  }
	
  void SavePowerBoxes() {
	JsonFileLoader<array<ref IE_FC_PowerBoxConfig>>.JsonSaveFile(POWER_BOX_PATH, power_boxes);
  }

  void MigrateLegacyVehicleAutonomy() {
	CF_Log.Info("[FuelControl] Loading legacy station configuration");
	map<string, float> vehicles = new map<string, float>;
    JsonFileLoader<map<string, float>>.JsonLoadFile(LEGACY_VEHICLE_AUTONOMY_PATH, vehicles);
	
	foreach(string key, float value: vehicles) {
		vehicle_config.Set(key,  new IE_FC_VehicleConfig(value, "GASOLINE"));
	}
	JsonFileLoader<map<string, ref IE_FC_VehicleConfig>>.JsonSaveFile(VEHICLE_CONFIG_PATH, vehicle_config );
	DeleteFile(LEGACY_VEHICLE_AUTONOMY_PATH);
  }

  void DefaultVehicleConfig() {
	vehicle_config.Set("CivilianSedan",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("CivilianSedan_Black", new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("CivilianSedan_Wine",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Hatchback_02",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Hatchback_02_Blue",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Hatchback_02_Black",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("OffroadHatchback",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("OffroadHatchback_Blue",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("OffroadHatchback_White", new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Sedan_02",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Sedan_02_Red", new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Sedan_02_Grey",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Truck_01_Covered",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Truck_01_Covered_Blue",  new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Truck_01_Covered_Orange", new IE_FC_VehicleConfig(25, "GASOLINE"));
    vehicle_config.Set("Offroad_02", new IE_FC_VehicleConfig(25, "GASOLINE"));
    JsonFileLoader<map<string, ref IE_FC_VehicleConfig>>.JsonSaveFile(VEHICLE_CONFIG_PATH, vehicle_config );
  }
	
  void DefaultLiquidTransferRates() {
    liquid_transfer_rates.Insert("Land_FuelStation_Feed",  1000.0);

    liquid_transfer_rates.Insert("CivilianSedan",  500.0);
    liquid_transfer_rates.Insert("CivilianSedan_Black",  500.0);
    liquid_transfer_rates.Insert("CivilianSedan_Wine",  500.0);
    liquid_transfer_rates.Insert("Hatchback_02",  500.0);
    liquid_transfer_rates.Insert("Hatchback_02_Blue",  500.0);
    liquid_transfer_rates.Insert("Hatchback_02_Black",  500.0);
    liquid_transfer_rates.Insert("OffroadHatchback",  500.0);
    liquid_transfer_rates.Insert("OffroadHatchback_Blue",  500.0);
    liquid_transfer_rates.Insert("OffroadHatchback_White",  500.0);
    liquid_transfer_rates.Insert("Sedan_02",  500.0);
    liquid_transfer_rates.Insert("Sedan_02_Red",  500.0);
    liquid_transfer_rates.Insert("Sedan_02_Grey",  500.0);
    liquid_transfer_rates.Insert("Truck_01_Covered",  500.0);
    liquid_transfer_rates.Insert("Truck_01_Covered_Blue",  800.0);
    liquid_transfer_rates.Insert("Truck_01_Covered_Orange",  800.0);
    liquid_transfer_rates.Insert("Offroad_02",  800.0);    


    liquid_transfer_rates.Insert("Barrel_Blue",  1000.0);
    liquid_transfer_rates.Insert("Barrel_Green",  1000.0);
    liquid_transfer_rates.Insert("Barrel_Red",  1000.0);
    liquid_transfer_rates.Insert("Barrel_Yellow",  1000.0);

    liquid_transfer_rates.Insert("CanisterGasoline",  500.0);
    liquid_transfer_rates.Insert("Canteen",  100.0);
    liquid_transfer_rates.Insert("Cauldron",  500.0);
    liquid_transfer_rates.Insert("Pot",  300.0);
    liquid_transfer_rates.Insert("WaterBottle",  100.0);
		
    JsonFileLoader<map<string, float>>.JsonSaveFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates );
  }
	
	void MigrateLegacyStations() {
		CF_Log.Info("[FuelControl] Loading legacy station configuration");
 		ref array<ref LegacyStationConfig> stations = new ref array<ref LegacyStationConfig>;
     	JsonFileLoader<array<ref LegacyStationConfig>>.JsonLoadFile(LEGACY_STATIONS_PATH, stations );
		foreach(auto legacyStation: stations) {
			auto fuels = new map<string, ref IEFCStationFuelConfig>;
			fuels.Set(IE_FC_StringForLiquid(LIQUID_GASOLINE), new IEFCStationFuelConfig(legacyStation.capacity));
			fuels.Set(IE_FC_StringForLiquid(LIQUID_DIESEL), new IEFCStationFuelConfig(-1));
			fuels.Set(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS), new IEFCStationFuelConfig(-1));
			auto station = new IEFCStationConfig(legacyStation.id, legacyStation.x, legacyStation.y, legacyStation.name, fuels);
			stations_config.Insert(station);
			
			auto fuelStates = new map<string, ref IEFCStationFuelState>;
			fuelStates.Set(IE_FC_StringForLiquid(LIQUID_GASOLINE), new IEFCStationFuelState(legacyStation.fuel));
			fuelStates.Set(IE_FC_StringForLiquid(LIQUID_DIESEL), new IEFCStationFuelState(0));
			fuelStates.Set(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS), new IEFCStationFuelState(0));
			auto stationState = new IEFCStationState(station.id, fuelStates);
			stations_state.Insert(stationState);
		}
		JsonFileLoader<array<ref IEFCStationConfig>>.JsonSaveFile(STATIONS_CONFIG_PATH, stations_config);
		JsonFileLoader<array<ref IEFCStationState>>.JsonSaveFile(STATIONS_STATE_PATH, stations_state);
		DeleteFile(LEGACY_STATIONS_PATH);
	}
	
  void DefaultStations() {
	auto fuels = new map<string, ref IEFCStationFuelConfig>;
	fuels.Set(IE_FC_StringForLiquid(LIQUID_GASOLINE), new IEFCStationFuelConfig(-1));
	fuels.Set(IE_FC_StringForLiquid(LIQUID_DIESEL), new IEFCStationFuelConfig(-1));
	fuels.Set(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS), new IEFCStationFuelConfig(-1));
	auto station = new IEFCStationConfig(FuelStationManager.GenId("Berezino"), 12977.8, 10076, "Berezino", fuels);
	stations_config.Insert(station);
	auto fuelStates = new map<string, ref IEFCStationFuelState>;
	fuelStates.Set(IE_FC_StringForLiquid(LIQUID_GASOLINE), new IEFCStationFuelState(0));
	fuelStates.Set(IE_FC_StringForLiquid(LIQUID_DIESEL), new IEFCStationFuelState(0));
	fuelStates.Set(IE_FC_StringForLiquid(IE_FC_LIQUID_AVGAS), new IEFCStationFuelState(0));
	stations_state.Insert(new IEFCStationState(station.id, fuelStates));
    CF_Log.Info("[FuelControl] Stations file doesn't exist, creating one");
    JsonFileLoader<array<ref IEFCStationConfig>>.JsonSaveFile(STATIONS_CONFIG_PATH, stations_config);
 	JsonFileLoader<array<ref IEFCStationState>>.JsonSaveFile(STATIONS_STATE_PATH, stations_state);
  }
	
  void DefaultPowerBoxes() {
	power_boxes.Insert(new ref IE_FC_PowerBoxConfig(FuelStationManager.GenId("Berezino"), 12977.8, 10076, -170, "Berezino",));
    CF_Log.Info("[FuelControl] Power boxes file doesn't exist, creating one");
    JsonFileLoader<array<ref IE_FC_PowerBoxConfig>>.JsonSaveFile(POWER_BOX_PATH, power_boxes);
  }

  void DefaultSettings() {
    CF_Log.Info("[FuelControl] Config file doesn't exist, creating one");
    JsonFileLoader<FCSettings>.JsonSaveFile(SETTINGS_PATH, settings);
  }
	

  void SetProp(string prop, string value) {
	TStringArray tokens = new TStringArray;
	prop.Split(".", tokens);

	switch (tokens[0]) {
		case "settings":
			switch (tokens[1]) {
				case "spawn_enabled":
					settings.spawn_enabled = value.ToInt();
					break;
				case "spawn_interval":
					settings.spawn_interval = value.ToInt();
					break;
				case "spawn_amount":
					settings.spawn_amount = value.ToInt();
					break;
				case "spawn_minimum":
					settings.spawn_minimum = value.ToInt();
					break;
				case "pump_car_refueling":
					settings.pump_car_refueling = value == "true";
					break;
				case "pump_barrel_refueling":
					settings.pump_barrel_refueling = value == "true";
					break;
				case "siphoning":
					settings.siphoning = value == "true";
					break;
				case "siphoning_limit":
					settings.siphoning_limit = value.ToFloat();
					break;
				case "default_pumps_have_fuel":
					settings.default_pumps_have_fuel = value == "true";
					break;
				case "pumps_get_ruined":
					settings.default_pumps_have_fuel = value == "true";
					break;
				case "measure_fuel_with_stick":
					settings.measure_fuel_with_stick = value == "true";
					break;
				case "pumps_require_electricity":
					settings.pumps_require_electricity = value == "true";
					break;
				case "pump_consumption":
					settings.pump_consumption = value.ToInt();
					break;
				default:
					CF_Log.Error("Unexpected prop " + prop + " setting on fuelconfig");
			}
			SaveSettings();
			break;
		case "vehicle_config":
			string vehicle_type = tokens[1];
			IE_FC_VehicleConfig config;
			switch (tokens[2]) {
				case "autonomy":
					auto autonomy = value.ToFloat();
					config = vehicle_config.Get(vehicle_type);
					if (config) {
						config.autonomy = autonomy;
					} else {
						config = new IE_FC_VehicleConfig(autonomy, IE_FC_StringForLiquid(LIQUID_GASOLINE));
					}
					vehicle_config.Set(vehicle_type, config);
					SaveVehicleConfig();
					break;
				case "fuel_type":
					auto fuel_type = value;
					config = vehicle_config.Get(vehicle_type);
					if (config) {
						config.fuel_type = fuel_type;
					} else {
						config = new IE_FC_VehicleConfig(25, fuel_type);
					}
					vehicle_config.Set(vehicle_type, config);
					SaveVehicleConfig();
					break;
				default:
					CF_Log.Error("Unexpected prop " + prop + " setting on fuelconfig");
			}
			break;
		case "liquid_transfer":
			liquid_transfer_rates.Set(tokens[1], value.ToFloat());
			break;
		case "powerbox":
			auto power_id = tokens[1];
			IE_FC_PowerBoxConfig powerbox;
			bool newbox = false;
			foreach (auto p: power_boxes) {
				if (p.id == power_id) {
					powerbox = p;
					break;
				}
			}
			if (!powerbox) {
				newbox = true;
				powerbox = new IE_FC_PowerBoxConfig(power_id, 0, 0, 0, "Unnamed");
			}
			  string id;

			switch (tokens[2]) {
				case "x":
					powerbox.x = value.ToFloat();
					break;
				case "y":
					powerbox.y = value.ToFloat();
					break;
				case "orientation":
					powerbox.orientation = value.ToFloat();
					break;
				case "name":
					powerbox.name = value;
					break;
				default:
					CF_Log.Error("Unexpected prop " + prop + " setting on fuelconfig");
			}
			
			if (newbox) {
				power_boxes.Insert(powerbox);
			}
			SavePowerBoxes();
			break;
		default:
			CF_Log.Error("Unexpected prop " + prop + " setting on fuelconfig");
	}
	if (GetGame().IsServer()) {
		SyncSetting(prop, value);
	}
  }
	
	void RemoveProp(string prop) {
		TStringArray tokens = new TStringArray;
		prop.Split(".", tokens);
		
		switch (tokens[0]) {
			case "vehicle_config":
				string vehicle_type = tokens[1];
				vehicle_config.Remove(vehicle_type);
				SaveVehicleConfig();
				break;
			case "liquid_transfer":
				string container_type = tokens[1];
				liquid_transfer_rates.Remove(container_type);
				SaveLiquidTransfer();
				break;
			case "powerbox":
				string box_id = tokens[1];
				for(int i = 0; i < power_boxes.Count(); i++) {
					if (power_boxes[i].id == box_id) {
						power_boxes.Remove(i);
						break;
					}
				}
				SavePowerBoxes();
				break;
			default:
				CF_Log.Error("Unexpected prop " + prop + " deletion on fuelconfig");
		}
		
		if (GetGame().IsServer()) {
			SyncRemoveSetting(prop);
		}
	}

  void SyncAll() {
	CF_Log.Debug("[FuelControl] Requesting all configs");
	GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", null, true);
  }

  void SyncSetting(string prop, string value) {
	CF_Log.Trace("[FuelControl] Sending update " + prop + " = " + value);
	GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>(prop, value), true);
  }
	
  void SyncRemoveSetting(string prop) {
	CF_Log.Trace("[FuelControl] Sending delete update " + prop);
	GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsDeleteSyncRPC", new Param1<string>(prop), true);
  }
	
  void FuelControlSettingsDeleteSyncRPC( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
    Param1<string> data;
    if (ctx.Read(data)) {
		CF_Log.Debug("[FuelControl] Got delete update " + data.param1);
		RemoveProp(data.param1);
    }
  }

  void FuelControlSettingsOnSyncRPC( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
    Param2<string, string> data;
    if (ctx.Read(data)) {
      CF_Log.Debug("[FuelControl] Got config update " + data.param1 + " = " + data.param2);
	  SetProp(data.param1, data.param2);
    } else if (GetGame().IsServer()) {
	  CF_Log.Debug("[FuelControl] Got config update request");
      // If the sender is not sending an update, then send all the info back to it.
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.spawn_enabled", "" + settings.spawn_enabled), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.spawn_interval", "" + settings.spawn_interval), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.spawn_amount", "" + settings.spawn_amount), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.spawn_minimum", "" + settings.spawn_minimum), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.pump_car_refueling", "" + settings.pump_car_refueling), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.pump_barrel_refueling", "" + settings.pump_barrel_refueling), true, sender, target);
      GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.siphoning", "" + settings.siphoning), true, sender, target);
      GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.siphoning_limit", "" + settings.siphoning_limit), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.default_pumps_have_fuel", "" + settings.default_pumps_have_fuel), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.pumps_get_ruined", "" + settings.pumps_get_ruined), true, sender, target);
      GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.measure_fuel_with_stick", "" + settings.measure_fuel_with_stick), true, sender, target);
      GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.pumps_require_electricity", "" + settings.pumps_require_electricity), true, sender, target);
	  GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("settings.pump_consumption", "" + settings.pump_consumption), true, sender, target);

	  foreach (string container_type, float transfer_rate: liquid_transfer_rates) {
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("liquid_transfer." + container_type, "" + transfer_rate), true, sender, target);
	  }
			
	  foreach (string vehicle_id, auto vehicle_conf: vehicle_config) {
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("vehicle_config." + vehicle_id + ".autonomy", "" + vehicle_conf.autonomy), true, sender, target);
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("vehicle_config." + vehicle_id + ".fuel_type" , vehicle_conf.fuel_type), true, sender, target);
	  }

	  foreach (auto powerbox: power_boxes) {
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("powerbox." + powerbox.id + ".x", "" + powerbox.x), true, sender, target);
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("powerbox." + powerbox.id + ".y", "" + powerbox.y), true, sender, target);
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("powerbox." + powerbox.id + ".orientation", "" + powerbox.orientation), true, sender, target);
			GetRPCManager().SendRPC("IE_FC", "FuelControlSettingsOnSyncRPC", new Param2<string, string>("powerbox." + powerbox.id + ".name", "" + powerbox.name), true, sender, target);
	  }
    }
  }
}

static ref FuelControlSettings g_FuelControlSettings;
static ref FuelControlSettings GetFuelControlSettings() {
    if (!g_FuelControlSettings) {
      g_FuelControlSettings = new ref FuelControlSettings();
	  if(GetGame().IsServer()) {
      	g_FuelControlSettings.Load();
	  }

      CF_Log.Info("[FuelControl] Loaded settings");
    }

    return g_FuelControlSettings;
}


