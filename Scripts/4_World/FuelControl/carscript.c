modded class CarScript {
	
	float fuelCapacity;
	float lastFuel = -1;
	float fuelAcc = 0;
	float consumptionRate;
	
	ScriptCallQueue queue = GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM); 
	
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillAtStation);
		AddAction(ActionSiphon);
	}
	
	override void OnEngineStart() {
		FuelControlSettings settings = GetFuelControlSettings();
		auto type = GetType();
		consumptionRate = settings.consumption_rates.Get(type);
		if (!consumptionRate) {
			consumptionRate = 1.5;
		}
		
		fuelCapacity = GetFluidCapacity( CarFluid.FUEL );

		queue.CallLater(this.CheckFuel, 1000, true);

		super.OnEngineStart();
	}
	
	override void OnEngineStop() {
		queue.Remove(this.CheckFuel);
		CheckFuel();
		super.OnEngineStop();
	}
	
	void CheckFuel() {

		if (consumptionRate != 1) {
			Fill(CarFluid.FUEL, 0.01);

			float fraction = GetFluidFraction( CarFluid.FUEL );
			float currentFuel = fraction * fuelCapacity;
			if (lastFuel == -1) {
				lastFuel = currentFuel;
			}
			float deltaFuel = lastFuel - currentFuel;
			fuelAcc += deltaFuel;
			if (fuelAcc >= 0.1) {
				float consumed = fuelAcc * consumptionRate;
				float diffConsumed = fuelAcc - consumed;
				if (diffConsumed < 0) {
					Leak(CarFluid.FUEL, diffConsumed * -1);
				} else if (diffConsumed > 0) {
					Fill(CarFluid.FUEL, diffConsumed);
				}
			
				fuelAcc = 0;
			}
			lastFuel = currentFuel;

		}
	}

};

/*
modded class Car: Transport {

		
	override void OnFluidChanged(CarFluid fluid, float newValue, float oldValue) {
		Print("Here " + fluid + " amount " + newValue);
	}
}
*/