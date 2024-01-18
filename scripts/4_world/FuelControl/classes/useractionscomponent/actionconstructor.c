modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		actions.Insert(ActionFillGasolineAtStation);
		actions.Insert(ActionFillDieselAtStation);
		actions.Insert(ActionFillAvGasAtStation);
		actions.Insert(ActionFillDieselFuel);
		actions.Insert(ActionFillGasolineFuel);
		actions.Insert(ActionFillAvGasFuel);
		actions.Insert(ActionSiphon);
		actions.Insert(ActionMeasureFuel);
		actions.Insert(IEActionTurnOn);
		actions.Insert(IEActionTurnOff);
		actions.Insert(ActionFillGasolineBottleBase);
		actions.Insert(ActionFillDieselBottleBase);
		actions.Insert(ActionFillAVGasBottleBase);
		super.RegisterActions(actions);
	}
};