modded class ActionFillBottleBase: ActionContinuousBase {

	FuelStation m_station;
	
	override string GetText(){
		if (m_station) {
			if (!m_station.HasFuel()) {
				return "There is no fuel at this station";
			} else if (!m_station.HasEnergy())  {
				return "Fuel pumps require energy to run";
			}
		}
		return super.GetText();		
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) {
		if (super.ActionCondition( player, target, item )) {
			m_station = FuelStation.Cast(target.GetObject());
			if (m_station){
				return !m_station.IsRuined();
			}
			return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			if (m_station){
				return !m_station.IsRuined() && m_station.HasFuel() && m_station.HasEnergy();
			}
			return true;
		}
		return false;
	}
	
	override void OnStartAnimationLoop(ActionData action_data) {
		super.OnStartAnimationLoop(action_data);
		if (GetGame().IsServer() && m_station && m_station.HasFuel() && m_station.HasEnergy()) {
			m_station.SetWorking(true);
		}
	}
	
	override void OnEndAnimationLoop(ActionData action_data) {
		super.OnEndAnimationLoop(action_data);
		if (GetGame().IsServer()) {
			if (m_station) {
				m_station.SetWorking(false);
			}
		}
	}
};

modded class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	
	override void CreateActionComponent() {
		
		FuelControlSettings settings = GetFuelControlSettings();
		float targetRate = settings.liquid_transfer_rates.Get(m_ActionData.m_Target.GetObject().GetType());
		if (!targetRate) {
			targetRate = UAQuantityConsumed.FUEL;
		}
		float itemRate = settings.liquid_transfer_rates.Get(m_ActionData.m_MainItem.GetType());
		if (!itemRate) {
			itemRate = UAQuantityConsumed.FILL_LIQUID;
		}
		
		float transferRate;
		
		if (targetRate < itemRate) {
			transferRate = targetRate;
		} else {
			transferRate = itemRate;
		}
		
		m_liquid_type = ActionFillBottleBase.Cast( m_ActionData.m_Action ).GetLiquidType( m_ActionData.m_Player, m_ActionData.m_Target, m_ActionData.m_MainItem );
		m_ActionData.m_ActionComponent = new CAContinuousFill(transferRate, m_liquid_type);
	}
};