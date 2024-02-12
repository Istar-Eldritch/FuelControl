modded class CarScript {
	
	static const string VEHICLE_LOOP_SOUND = "IE_FC_VehicleRefueling_SoundSet";
	protected EffectSound 	m_RefuelingSound;
	
	float m_lastFuelAmount;
	float autonomy;
	int fuel_type;
	// During a dt teh amount of fuel may be so small the leak function doesn't compute it right.
	// This is an acc across multiple updates.
	float fuelDebt;
	bool m_Refueling;
	
	void CarScript() {
		RegisterNetSyncVariableBool("m_Refueling");
		ReloadConfigs();
	}
	
	override void OnVariablesSynchronized() {
		super.OnVariablesSynchronized();
		if (GetGame().IsClient()) {
			if (m_Refueling) {
				PlaySoundSetLoop(m_RefuelingSound, VEHICLE_LOOP_SOUND, 0, 0);
			} else {
				StopSoundSet(m_RefuelingSound);
			}
		}
	}
	
	int GetFuelType() {
		return fuel_type;
	}
	
	void SetRefueling(bool refueling) {
		m_Refueling = refueling;
		SetSynchDirty();
	}
		
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillGasolineAtStation);
		AddAction(ActionFillDieselAtStation);
		AddAction(ActionFillAvGasAtStation);
		AddAction(ActionSiphon);
	}
	
	void AddFuel(float quantity) {
		Fill(CarFluid.FUEL, quantity);
		m_lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
		SetSynchDirty();
	}
	
	void RemoveFuel(float quantity) {
		Leak(CarFluid.FUEL, quantity);
		m_lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
		SetSynchDirty();
	}
	
	float GetFuelAmount() {
		return GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
	}
	
	void ReloadConfigs() {
		FuelControlSettings settings = GetFuelControlSettings();
		auto type = GetType();
		auto vehicle_config = settings.vehicle_config.Get(type);
		if (vehicle_config) {
			autonomy = vehicle_config.autonomy;
			fuel_type = IE_FC_LiquidFromString(vehicle_config.fuel_type);
			if (fuel_type == -1) {
				CF_Log.Error("[FuelControl] Couldn't parse fuel type for " + type + " likely a misconfiguration");
				fuel_type = LIQUID_GASOLINE;
			}
		} else {
			autonomy = 0;
			fuel_type = LIQUID_GASOLINE;
		}
	}
	
	override void OnEngineStart() {
		fuelDebt = 0;
		float fuelFraction = GetFluidFraction( CarFluid.FUEL);
		m_lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * fuelFraction;
		ReloadConfigs();

		super.OnEngineStart();
	}
	
	override void OnUpdate(float dt) {
		super.OnUpdate(dt);

		if (GetGame().IsServer() && autonomy > 0 && EngineIsOn()) {
			float fuelFraction = GetFluidFraction( CarFluid.FUEL );
			float currentFuelAmount = GetFluidCapacity(CarFluid.FUEL) * fuelFraction;
			float fuelTankHealth = GetHealth01("FuelTank", "");
			// Disable vanilla consumption if autonomy rate is defined and the fuel tank is healthty.
			// Otherwise damaged tanks would not leak
			if (currentFuelAmount < m_lastFuelAmount && fuelTankHealth > GameConstants.DAMAGE_DAMAGED_VALUE) {
				float consumed = m_lastFuelAmount - currentFuelAmount;
				AddFuel(consumed);
			}

			float rpm;
			#ifdef RFFS_HELI
			RFFSHeli_base heli = RFFSHeli_base.Cast(this);
			if (heli) {
				rpm = heli.m_collective_level / 20;
			}
			#endif
			#ifdef RFWC
			RFWC_base boat = RFWC_base.Cast(this);
			if (boat) {
				rpm = Math.AbsFloat(boat.m_throttle_level / 20);
			}
			#endif
			if (!rpm) {
				rpm = EngineGetRPM() / EngineGetRPMMax();
			}
			
			if (rpm < 0.1) {
				rpm = 0.1;
			}
			
			
			float speed = GetVelocity(this).Length(); // speed in m/s
			float ds = dt * speed; // distance traveled in dt in m
			
			// fuel consumed in ds
			float consumedFuel = (ds * GetFluidCapacity(CarFluid.FUEL) / autonomy / 1000 + fuelDebt) * (rpm + 0.5);

			if (consumedFuel > 1e-3) {
				RemoveFuel(consumedFuel);
				fuelDebt = 0;
			} else {
				fuelDebt = consumedFuel;
			}
		}
	}
	
	override void OnEngineStop() {
		super.OnEngineStop();
	}

};
