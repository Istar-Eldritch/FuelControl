modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		actions.Insert(ActionFillAtStation);
		actions.Insert(ActionSiphon);
		actions.Insert(ActionMeasureFuel);
		super.RegisterActions(actions);
	}
};