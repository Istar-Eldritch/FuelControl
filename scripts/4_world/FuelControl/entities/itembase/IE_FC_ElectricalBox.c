class IE_FC_StationPowerSource: BuildingSuper {
	
	protected FuelStationGroup group = null;

	void IE_FC_StationPowerSource() {
		Print("PowerSource constructor");
	}

	override bool IsInventoryVisible() {
		return true;
	}
	
	override bool IsHealthVisible() {
		return true;
	}

	override bool CanPutIntoHands(EntityAI player) {
		return true;
	}

	override bool CanPutInCargo(EntityAI parent) {
		return true;
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

class IE_FC_ElectricalBox: IE_FC_StationPowerSource {}