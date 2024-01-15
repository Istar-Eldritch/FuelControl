modded class FuelStation {
	
	protected FuelStationGroup group = null;
	
	void FuelStation() {
		RegisterNetSyncVariableBool("m_ruined");
	}
	
	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
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
	
	bool HasEnergy() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}

		return group.HasEnergy();
	}
	
	override bool IsRuined() {
		if (IsRuined()) {
			auto config = GetFuelControlSettings();
			return config.settings.pumps_get_ruined;
		}
		return false;
	}
	
	override void SetActions()
	{
		super.SetActions();
	}
};