modded class CarScript {
	
	float fuelCapacity;
	float lastFuelFraction;
	float consumptionRate;
		
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillAtStation);
		AddAction(ActionSiphon);
	}
	
	void AddFuel(float quantity) {
		Fill(CarFluid.FUEL, quantity);
		lastFuelFraction = GetFluidFraction(CarFluid.FUEL);
	}
	
	void RemoveFuel(float quantity) {
		Leak(CarFluid.FUEL, quantity);
		lastFuelFraction = GetFluidFraction(CarFluid.FUEL);
	}
	
	override void OnEngineStart() {
		fuelCapacity = GetFluidCapacity( CarFluid.FUEL );
		lastFuelFraction = GetFluidFraction( CarFluid.FUEL);
		FuelControlSettings settings = GetFuelControlSettings();
		auto type = GetType();
		consumptionRate = settings.consumption_rates.Get(type);
		if (!consumptionRate) {
			consumptionRate = 1;
		}

		super.OnEngineStart();
	}
	
	override void OnUpdate(float dt) {
		super.OnUpdate(dt);
		float fuelFraction = GetFluidFraction( CarFluid.FUEL );
		
		// This will potentially affect refueling
		if (consumptionRate != 1 && EngineIsOn() && fuelFraction != lastFuelFraction) {
			float lastFuel = lastFuelFraction * fuelCapacity;
			float fuel = fuelFraction * fuelCapacity;
			float consumed = lastFuel - fuel;
			if (consumed > 0) {
				float shouldBeConsumed = consumed * consumptionRate;
				float diffConsumed = consumed - shouldBeConsumed;
				
				if (diffConsumed < 0) {
					RemoveFuel((diffConsumed * -1));
				} else if (diffConsumed > 0) {
					AddFuel(diffConsumed);
				}
			}
		}
	}
	
	override void OnEngineStop() {
		super.OnEngineStop();
	}

};
