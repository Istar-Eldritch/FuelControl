modded class Barrel_ColorBase: DeployableContainer_Base {
		
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillGasolineAtStation);
		AddAction(ActionFillDieselAtStation);
		AddAction(ActionFillAvGasAtStation);
	}
}