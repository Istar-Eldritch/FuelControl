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
	
	bool HasFuel() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			FuelStationGroup g = groupManager.FindStationForPump(this.GetPosition());
			if (g) {
				Print("[FuelControl] Pump at " + this.GetPosition() + " at " + g.name + " station");
				group = g;
			} else {
				Print("[FuelControl] Pump at " + this.GetPosition() + " belongs to no station ");
			}
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
