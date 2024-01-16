class IE_FC_StationPowerSource: BuildingSuper {
	

	void IE_FC_StationPowerSource() {
		if (GetGame().IsServer()) {
			FuelStationManager groupManager = GetFuelStationManager();
			FuelStationGroup station = groupManager.FindStationForPump(GetPosition());
			if (station) {
				auto existing = station.GetPowerSource();
				if (existing) {
					Delete();
				} else {
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(PlaceOnStationPoint);
				}
			}
		}
	}
	
	void PlaceOnStationPoint() {
		FuelStationManager groupManager = GetFuelStationManager();
		FuelStationGroup station = groupManager.FindStationForPump(GetPosition());
		if (station) {
			SetPosition(FCTeleportManager.SnapToGround(station.position));
			vector orientation = "0 0 0";
			orientation[0] = station.m_orientation;
			SetOrientation(orientation);
		}
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