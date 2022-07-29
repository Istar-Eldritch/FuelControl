modded class FuelStation {
	
	protected FuelStationGroup group = null;
	
	void FuelStation() {

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

};
