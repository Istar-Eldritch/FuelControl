class FuelSpawner {
    ref Timer m_SpawnTimer;

    void FuelSpawner() {
        m_SpawnTimer = new Timer();
    }

    void Start() {
        Print("[FuelControl] Started spawner");
        auto settings = GetFuelControlSettings().settings;
        m_SpawnTimer.Run(settings.spawn_interval, this, "SpawnFuel", NULL, true);
    }

    void SpawnFuel() {
        Print("[FuelControl] Spawning fuel");
        auto settings = GetFuelControlSettings().settings;
        GetFuelStationManager().Spawn(settings.spawn_amount);
    }
}

static ref FuelSpawner g_FuelSpawner;
static ref FuelSpawner GetFuelSpawner() {
    if (!g_FuelSpawner) {
        g_FuelSpawner = new ref FuelSpawner();
    }

    return g_FuelSpawner;
}
