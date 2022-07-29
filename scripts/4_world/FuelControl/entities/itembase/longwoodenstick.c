modded class LongWoodenStick: Inventory_Base {
	override void SetActions() {
		super.SetActions();
		AddAction(ActionMeasureFuel);
	}
};