class StationPowerSource: House {
	
	protected FuelStationGroup group = null;
	
	void StationPowerSource() {
		Print("Init power source");
	}

    override void OnSwitchOn() {
		FuelControlSettings settings =  GetFuelControlSettings();
	}
	
	override void OnWorkStart() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		if (group)
			group.SetHasEnergy(true);
	}
	
	override void OnWorkStop() {
		if (!group) {
			FuelStationManager groupManager = GetFuelStationManager();
			group = groupManager.FindStationForPump(this.GetPosition());
		}
		if (group)
			group.SetHasEnergy(false);
	}
	
	override bool IsElectricAppliance() {
		return true;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(IEActionTurnOn);
		AddAction(IEActionTurnOff);
	}
}

class IEFCElectricBox: StationPowerSource {}