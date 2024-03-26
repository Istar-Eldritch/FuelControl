modded class Bottle_Base {

	override void SetActions() {
		super.SetActions();
		RemoveAction(ActionFillFuel);
		AddAction(ActionFillDieselFuel);
		AddAction(ActionFillGasolineFuel);
		AddAction(ActionFillAvGasFuel);
		AddAction(ActionFillGasolineBottleBase);
		AddAction(ActionFillDieselBottleBase);
		AddAction(ActionFillAVGasBottleBase);
	}
}