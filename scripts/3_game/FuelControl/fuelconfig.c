class StationConfig {
	float x;
	float y;
	string name;
	float fuel = -1; // If negative then there is no limit on this station
	float capacity = -1;

	void StationConfig(float xx, float yy, string _name, float _capacity, float _fuel) {
		x = xx;
		y = yy;
		name = _name;
		fuel = _fuel;
		capacity = _capacity;
	}
}

class Settings {
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
	static private const string CONSUMPTION_RATES_PATH = DIR_PATH + "\\consumption_rates.json";
	static private const string LIQUID_TRANSFER_RATES_PATH = DIR_PATH + "\\liquid_transfer_rates.json";
	
	ref Settings settings = new ref Settings();
	ref array<ref StationConfig> stations = new ref array<ref StationConfig>;
	ref map<string, float> consumption_rates = new ref map<string, float>;
	ref map<string, float> liquid_transfer_rates = new ref map<string, float>;
	
	void Load() {
		if (FileExist(SETTINGS_PATH)){ //If config exist load File
			Print("[FuelControl] Loading configuration");
			JsonFileLoader<Settings>.JsonLoadFile(SETTINGS_PATH, settings );
			JsonFileLoader<array<ref StationConfig>>.JsonLoadFile(STATIONS_PATH, stations );
			JsonFileLoader<map<string, float>>.JsonLoadFile(CONSUMPTION_RATES_PATH, consumption_rates );
			JsonFileLoader<map<string, float>>.JsonLoadFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates );
		} else if (GetGame().IsServer()) { //File does not exist use default settings and create file.
			DefaultSettings();
		}
	}
	
	void Save() {
		JsonFileLoader<array<ref StationConfig>>.JsonSaveFile(STATIONS_PATH, stations );
	}

	void DefaultSettings() {
		Print("[FuelControl] Using default settings");
		stations.Insert(new ref StationConfig(5861, 2210, "Cherno West", -1, -1));
		stations.Insert(new ref StationConfig(6872, 3092, "Cherno East", -1, -1));
		
	    consumption_rates.Insert("CivilianSedan",  1);
	    consumption_rates.Insert("CivilianSedan_Black",  1);
	    consumption_rates.Insert("CivilianSedan_Wine",  1);
	    consumption_rates.Insert("Hatchback_02",  1);
	    consumption_rates.Insert("Hatchback_02_Blue",  1);
	    consumption_rates.Insert("Hatchback_02_Black",  1);
	    consumption_rates.Insert("OffroadHatchback",  1);
	    consumption_rates.Insert("OffroadHatchback_Blue",  1);
	    consumption_rates.Insert("OffroadHatchback_White", 1);
	    consumption_rates.Insert("Sedan_02",  1);
	    consumption_rates.Insert("Sedan_02_Red",  1);
	    consumption_rates.Insert("Sedan_02_Grey",  1);
	    consumption_rates.Insert("Truck_01_Covered",  1);
	    consumption_rates.Insert("Truck_01_Covered_Blue",  1);
	    consumption_rates.Insert("Truck_01_Covered_Orange", 1);
		
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
	
	    liquid_transfer_rates.Insert("Barrel_Blue",  1000.0);
	    liquid_transfer_rates.Insert("Barrel_Green",  1000.0);
	    liquid_transfer_rates.Insert("Barrel_Red",  1000.0);
	    liquid_transfer_rates.Insert("Barrel_Yellow",  1000.0);
	
	    liquid_transfer_rates.Insert("CanisterGasoline",  500.0);
	    liquid_transfer_rates.Insert("Canteen",  100.0);
	    liquid_transfer_rates.Insert("Cauldron",  500.0);
	    liquid_transfer_rates.Insert("Pot",  300.0);
	    liquid_transfer_rates.Insert("WaterBottle",  100.0);
		
		Print("[FuelControl] Config file doesn't exist, creating one");
		if (!FileExist(DIR_PATH)){
			MakeDirectory(DIR_PATH);
		}
		
		JsonFileLoader<Settings>.JsonSaveFile(SETTINGS_PATH, settings);
		JsonFileLoader<array<ref StationConfig>>.JsonSaveFile(STATIONS_PATH, stations);
		JsonFileLoader<map<string, float>>.JsonSaveFile(CONSUMPTION_RATES_PATH, consumption_rates );
		JsonFileLoader<map<string, float>>.JsonSaveFile(LIQUID_TRANSFER_RATES_PATH, liquid_transfer_rates );
	}
	
	void SyncSettings() {
		Print("[FuelControl] Requesting settings from server");
		GetRPCManager().SendRPC("FuelControl", "GetSettings", null, true);
	}
	
	void GetSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<FuelControlSettings> data;
		if (ctx.Read(data)) {
			ref FuelControlSettings config = data.param1;

			settings = config.settings;
			consumption_rates = config.consumption_rates;
			foreach(auto k, auto station: config.stations) {
		 		stations.Insert(new ref StationConfig(station.x, station.y, station.name, station.capacity, station.fuel));
			}
			liquid_transfer_rates = config.liquid_transfer_rates;
			Print("[FuelControl] Got config from server");

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
    }

    return g_FuelControlSettings;
}


