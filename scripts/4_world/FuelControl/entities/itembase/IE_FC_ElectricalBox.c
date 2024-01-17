class IE_FC_StationPowerSource: BuildingSuper {
	
	override bool IsInventoryVisible() {
		return true;
	}
	
	override bool IsHealthVisible() {
		return true;
	}
	
	override bool IsElectricAppliance() {
		return true;
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(IEActionTurnOn);
		RemoveAction(IEActionTurnOff);
	}
}

class IE_FC_ElectricalBox: IE_FC_StationPowerSource {}