static int IE_FC_STATION_CNT = 0;

modded class FuelStation {
	static const string	STATION_LOOP_SOUND = "IE_FC_PumpRefueling_SoundSet";
	protected EffectSound 	m_StationSoundLoop;

	void SetWorking(bool working) {
		GetRPCManager().SendRPC("IE_FC", "FuelStationSoundUpdate", new Param2<vector, bool>(GetWorldPosition(), working), true);
  	}
	
	void PlayFuelingSound(bool play) {
		if (play) {
			PlaySoundSetLoop(m_StationSoundLoop, STATION_LOOP_SOUND, 0, 0);
		} else {
			StopSoundSet(m_StationSoundLoop);
		}
	}
	
	float GetFuel(int fuelType) {
		auto config = GetFuelControlSettings();

		FuelStationManager groupManager = GetFuelStationManager();
		FuelStationGroup group = groupManager.FindStationForPump(this.GetPosition());
		
		if(group) {
			return group.GetFuel(fuelType);
		}
		
		if(config.settings.default_pumps_have_fuel) {
			return -1;
		}

		return 0;
	}
	
	bool HasFuel(int fuelType) {
		FuelStationManager groupManager = GetFuelStationManager();
		FuelStationGroup group = groupManager.FindStationForPump(this.GetPosition());
		
		if (group) {
			return group.HasFuel(fuelType);
		}

		auto config = GetFuelControlSettings();

		return config.settings.default_pumps_have_fuel;
	}

	void RemoveFuel(int liquidType, float quantity) {
		FuelStationManager groupManager = GetFuelStationManager();
		FuelStationGroup group = groupManager.FindStationForPump(this.GetPosition());
		group.RemoveFuel(liquidType, quantity);
	}
	
	bool HasEnergy() {
		auto config = GetFuelControlSettings();

		if (config.settings.pumps_require_electricity) {
			FuelStationManager groupManager = GetFuelStationManager();
			FuelStationGroup group = groupManager.FindStationForPump(this.GetPosition());
	
			if (group) {
				return group.HasEnergy();
			} else {
				return false;
			}
		} else {
			return true;
		}
	}
	
	override bool IsRuined() {
		if (super.IsRuined()) {
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