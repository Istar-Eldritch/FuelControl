modded class ActionConstructor {

	override void RegisterActions(TTypenameArray actions) {
		super.RegisterActions(actions);
		actions.Insert(ActionFillAtStation);
		actions.Insert(ActionSiphon);
	}
};

modded class CarScript {
	override void SetActions() {
		super.SetActions();
		AddAction(ActionFillAtStation);
		AddAction(ActionSiphon);
	}
};