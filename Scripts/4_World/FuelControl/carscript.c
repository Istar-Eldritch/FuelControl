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

		/*
		queue.CallLater(this.CheckFuel, 1000, true);
		*/

		super.OnEngineStart();
	}
	
	override void OnFluidChanged( CarFluid fluid, float newValue, float oldValue ) {
		Print("Fluid changed: " + fluid + " " + newValue + " " + oldValue);
	}
	
	override void OnUpdate(float dt) {
		super.OnUpdate(dt);
		float fuelFraction = GetFluidFraction( CarFluid.FUEL );
		
		// This will potentially affect refueling
		if (consumptionRate != 1 && EngineIsOn() && fuelFraction != lastFuelFraction) {
			float lastFuel = lastFuelFraction * fuelCapacity;
			float fuel = fuelFraction * fuelCapacity;
			
			float consumed = lastFuel - fuel;
			float shouldBeConsumed = consumed * consumptionRate;
			float diffConsumed = consumed - shouldBeConsumed;
			
			if (diffConsumed < 0) {
				RemoveFuel((diffConsumed * -1));
			} else if (diffConsumed > 0) {
				AddFuel(diffConsumed);
			}
			
			Print("LastFraction: " + lastFuelFraction);
			Print("Fraction: " + fuelFraction);
			Print("consumed" + consumed); 
			Print("shouldBeConsumed" + shouldBeConsumed);
			Print("Diffconsumed" + diffConsumed);
		}
	}
	
	override void OnEngineStop() {
		/*
		queue.Remove(this.CheckFuel);
		CheckFuel();
		*/
		super.OnEngineStop();
	}

};

/*
modded class Car: Transport {

		
	override void OnFluidChanged(CarFluid fluid, float newValue, float oldValue) {
		Print("Here " + fluid + " amount " + newValue);
	}
}
*/