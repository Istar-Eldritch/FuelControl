class StationPowerSource: House {
	
	void StationPowerSource() {
		Print("Init power source");
	}
	
	override bool IsElectricAppliance() {
		return true;
	}

	override void SetActions()
	{
		super.SetActions();
	}
}

class IEFCElectricBox: StationPowerSource {}