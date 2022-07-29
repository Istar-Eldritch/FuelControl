modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		super.RegisterActions(actions);
		actions.Insert(ActionFillAtStation);
		actions.Insert(ActionSiphon);
		actions.Insert(ActionMeasureFuel);
	}
};