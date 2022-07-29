modded class FuelStation {
	
	protected FuelStationGroup group = null;
	protected bool ruined = false;
	
	void FuelStation() {
		RegisterNetSyncVariableBool("ruined");
	}

	override void EEKilled(Object killer) {
		super.EEKilled(killer);
		auto config = GetFuelControlSettings();
		if(config.settings.pumps_get_ruined) {
			ruined = true;
			SetSynchDirty();
		}
	}
	
	float GetFuel() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		
		if(group) {
			return group.GetFuel();
		}
		
		auto config = GetFuelControlSettings();

		if(config.settings.default_pumps_have_fuel) {
			return -1;
		}

		return 0;
	}
	
	bool HasFuel() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		
		if (group) {
			return group.HasFuel();
		}
		auto config = GetFuelControlSettings();

		return config.settings.default_pumps_have_fuel;
	}

	void RemoveFuel(float quantity) {
		if (group) {
			group.RemoveFuel(quantity);
		}
	}
	
	override bool IsRuined() {
		return ruined;
	}

};
