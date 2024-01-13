modded class CarScript {
	
	static const string VEHICLE_LOOP_SOUND = "IE_FC_VehicleRefueling_SoundSet";
	protected EffectSound 	m_RefuelingSound;
	
	float lastFuelAmount;
	float autonomy;
	// During a dt teh amount of fuel may be so small the leak function doesn't compute it right.
	// This is an acc across multiple updates.
	float fuelDebt;
	bool m_Refueling;
	
	void CarScript() {
		RegisterNetSyncVariableBool("m_Refueling");
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
	
	void SetRefueling(bool refueling) {
		m_Refueling = refueling;
		SetSynchDirty();
	}
		
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillAtStation);
		AddAction(ActionSiphon);
		AddAction(ActionMeasureFuel);
	}
	
	void AddFuel(float quantity) {
		Fill(CarFluid.FUEL, quantity);
		lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
	}
	
	void RemoveFuel(float quantity) {
		Leak(CarFluid.FUEL, quantity);
		lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
	}
	
	override void OnEngineStart() {
		fuelDebt = 0;
		float fuelFraction = GetFluidFraction( CarFluid.FUEL);
		lastFuelAmount = GetFluidCapacity(CarFluid.FUEL) * fuelFraction;
		FuelControlSettings settings = GetFuelControlSettings();
		auto type = GetType();
		autonomy = settings.vehicle_autonomy.Get(type);
		if (!autonomy) {
			autonomy = 0;
		}

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
			if (currentFuelAmount < lastFuelAmount && fuelTankHealth > GameConstants.DAMAGE_DAMAGED_VALUE) {
				float consumed = lastFuelAmount - currentFuelAmount;
				AddFuel(consumed);
			}
			
			float rpm = EngineGetRPM() / EngineGetRPMMax();
			
			
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
