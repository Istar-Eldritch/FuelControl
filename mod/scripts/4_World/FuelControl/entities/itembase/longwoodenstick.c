modded class LongWoodenStick: Inventory_Base {
	override void SetActions() {
		super.SetActions();
		AddAction(ActionMeasureGasoline);
		AddAction(ActionMeasureDiesel);
		AddAction(ActionMeasureAvGas);
	}
};