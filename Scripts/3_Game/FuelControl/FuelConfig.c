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


class FuelControlSettings {
	
	static private const string DIR_PATH = "$profile:FuelControl";
	static private const string SETTINGS_PATH = DIR_PATH + "\\settings.json";

	ref array<ref StationConfig> stations;
	
	void FuelControlSettings() {
		stations = new ref array<ref StationConfig>
	}
	
	void Load() {
		if (FileExist(SETTINGS_PATH)){ //If config exist load File
			Print("[FuelControl] Loading configuration");
			JsonFileLoader<FuelControlSettings>.JsonLoadFile(SETTINGS_PATH, this );
		} else{ //File does not exist use default settings and create file.
			DefaultSettings();
		}
	}

	void DefaultSettings() {
		Print("[FuelControl] Using default settings");
		stations.Insert(new ref StationConfig(5861, 2210, "Cherno West", -1, -1));
		stations.Insert(new ref StationConfig(6872, 3092, "Cherno North", -1, -1));
		
		Print("[FuelControl] Config file doesn't exist, creating one");
		if (!FileExist(DIR_PATH)){
			MakeDirectory(DIR_PATH);
		}
		JsonFileLoader<FuelControlSettings>.JsonSaveFile(SETTINGS_PATH, this);
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


