class StationConfig {
  string id;
  float x;
  float y;
  string name;
  float fuel = -1; // If negative then there is no limit on this station
  float capacity = -1;

  void StationConfig(string _id, float xx, float yy, string _name, float _capacity, float _fuel) {
	id = _id;
    x = xx;
    y = yy;
    name = _name;
    fuel = _fuel;
    capacity = _capacity;
  }
}

class FCSettings {
  ref array<string> administrators = new ref array<string>;
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
}


class FuelControlSettings {
  
  static private const string DIR_PATH = "$profile:FuelControl";
  static private const string SETTINGS_PATH = DIR_PATH + "\\settings.json";
  static private const string STATIONS_PATH = DIR_PATH + "\\stations.json";
  static private const string VEHICLE_AUTONOMY_PATH = DIR_PATH + "\\vehicle_autonomy.json";
  static private const string LIQUID_TRANSFER_RATES_PATH = DIR_PATH + "\\liquid_transfer_rates.json";
  
  ref FCSettings settings = new ref FCSettings();
  ref array<ref StationConfig> stations = new ref array<ref StationConfig>;
  ref map<string, float> vehicle_autonomy = new ref map<string, float>;
  ref map<string, float> liquid_transfer_rates = new ref map<string, float>;
  
  void Load() {
    if (!FileExist(DIR_PATH)){
      MakeDirectory(DIR_PATH);
    }

    if (FileExist(SETTINGS_PATH)){ //If config exist load File
      Print("[FuelControl] Loading configuration");
      JsonFileLoader<FCSettings>.JsonLoadFile(SETTINGS_PATH, settings );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultSettings();
    }
		
    if (FileExist(STATIONS_PATH)){ //If config exist load File
      Print("[FuelControl] Loading station configuration");
      JsonFileLoader<array<ref StationConfig>>.JsonLoadFile(STATIONS_PATH, stations );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultStations();
    }
		
	if (FileExist(VEHICLE_AUTONOMY_PATH)){ //If config exist load File
      Print("[FuelControl] Loading vehicle autonomy configuration");
      JsonFileLoader<map<string, float>>.JsonLoadFile(VEHICLE_AUTONOMY_PATH, vehicle_autonomy );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultVehicleAutonomy();
    }
		
	if (FileExist(LIQUID_TRANSFER_RATES_PATH)){ //If config exist load File
      Print("[FuelControl] Loading liquid transfer rates configuration");
      JsonFileLoader<map<string, float>>.JsonLoadFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates );
    } else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
      DefaultLiquidTransferRates();
    }
  }
  
  void Save() {
    JsonFileLoader<array<ref StationConfig>>.JsonSaveFile(STATIONS_PATH, stations );
	JsonFileLoader<FCSettings>.JsonSaveFile(SETTINGS_PATH, settings);
	JsonFileLoader<map<string, float>>.JsonSaveFile(VEHICLE_AUTONOMY_PATH, vehicle_autonomy);
  }
	
  void DefaultVehicleAutonomy() {
	vehicle_autonomy.Insert("CivilianSedan",  0);
    vehicle_autonomy.Insert("CivilianSedan_Black",  0);
    vehicle_autonomy.Insert("CivilianSedan_Wine",  0);
    vehicle_autonomy.Insert("Hatchback_02",  0);
    vehicle_autonomy.Insert("Hatchback_02_Blue",  0);
    vehicle_autonomy.Insert("Hatchback_02_Black",  0);
    vehicle_autonomy.Insert("OffroadHatchback",  0);
    vehicle_autonomy.Insert("OffroadHatchback_Blue",  0);
    vehicle_autonomy.Insert("OffroadHatchback_White", 0);
    vehicle_autonomy.Insert("Sedan_02",  0);
    vehicle_autonomy.Insert("Sedan_02_Red", 0);
    vehicle_autonomy.Insert("Sedan_02_Grey",  0);
    vehicle_autonomy.Insert("Truck_01_Covered",  0);
    vehicle_autonomy.Insert("Truck_01_Covered_Blue",  0);
    vehicle_autonomy.Insert("Truck_01_Covered_Orange", 0);
    vehicle_autonomy.Insert("Offroad_02", 0);
    JsonFileLoader<map<string, float>>.JsonSaveFile(VEHICLE_AUTONOMY_PATH, vehicle_autonomy );
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
	
  void DefaultStations() {
    stations.Insert(new ref StationConfig(FuelStationManager.GenId("Cherno West"), 5861, 2210, "Cherno West", -1, -1));
    stations.Insert(new ref StationConfig(FuelStationManager.GenId("Cherno East"), 6872, 3092, "Cherno East", -1, -1));
    Print("[FuelControl] Stations file doesn't exist, creating one");
    JsonFileLoader<array<ref StationConfig>>.JsonSaveFile(STATIONS_PATH, stations);
  }

  void DefaultSettings() {
    Print("[FuelControl] Config file doesn't exist, creating one");
    JsonFileLoader<FCSettings>.JsonSaveFile(SETTINGS_PATH, settings);
  }
  
  void SyncSettings(bool push = false) {
	if (push) {
		Print("[FuelControl] Sending update to server");
		GetRPCManager().SendRPC("FuelControl", "GetSettings", new Param1<FuelControlSettings>(this), true);
	} else {
		Print("[FuelControl] Requesting settings from server");
		GetRPCManager().SendRPC("FuelControl", "GetSettings", null, true);	
	}
  }
  
  void GetSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
    Param1<FuelControlSettings> data;
    if (ctx.Read(data)) {
      ref FuelControlSettings config = data.param1;

      settings = config.settings;
      vehicle_autonomy = config.vehicle_autonomy;
      foreach(auto k, auto station: config.stations) {
         stations.Insert(new ref StationConfig(station.id, station.x, station.y, station.name, station.capacity, station.fuel));
      }
      liquid_transfer_rates = config.liquid_transfer_rates;
      Print("[FuelControl] Got config update");
	  if (GetGame().IsServer()) {
		Save();	
	  }
    } else if (GetGame().IsServer()) {
      // If the sender is not sending an update, then send all the station information back to it.
      GetRPCManager().SendRPC("FuelControl", "GetSettings", new Param1<FuelControlSettings>(this), true, sender, target);
    }
  }
  
}

static ref FuelControlSettings g_FuelControlSettings;
static ref FuelControlSettings GetFuelControlSettings() {
    if (!g_FuelControlSettings) {
      g_FuelControlSettings = new ref FuelControlSettings();
      g_FuelControlSettings.Load();

      Print("[FuelControl] Loaded settings");
    }

    return g_FuelControlSettings;
}


