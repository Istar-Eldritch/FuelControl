modded class ActionFillBottleBase {
	override int GetLiquidType(PlayerBase player, ActionTarget target, ItemBase item) {
		int res = super.GetLiquidType(player, target, item);
		if (res == LIQUID_GASOLINE) {
			return -1;
		}
		return res;
	}
};

class ActionFillAtStationBottleBase: ActionFillBottleBase {

	FuelStation m_station;
	
	int m_liquid = -1;
	
	override int GetLiquidType(PlayerBase player, ActionTarget target, ItemBase item) {
		if (target.GetObject() && target.GetObject().IsFuelStation())
		{
			if (Liquid.CanFillContainer(item, m_liquid))
				return m_liquid;
		}

		return -1;
	}
	
	override string GetText(){
		if (m_station) {
			string fuelname = IE_FC_UIStringForLiquid(m_liquid);
			fuelname.ToLower();
			if (!m_station.HasFuel(m_liquid)) {
				string noFuelLoc = Widget.TranslateString( "#STR_IEFC_NO_FUELTYPE" );
				noFuelLoc.Replace("{fuel_type}", fuelname);
				return noFuelLoc;
			} else if (!m_station.HasEnergy())  {
				string noEnergyLoc = Widget.TranslateString( "#STR_IEFC_NO_ENERGY" );
				return noEnergyLoc;
			} else {
				string fillLoc = Widget.TranslateString( "#STR_IEFC_FILL_FUELTYPE" );
				fillLoc.Replace("{fuel_type}", fuelname);
				return fillLoc;
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
				return !m_station.IsRuined() && m_station.HasFuel(m_liquid) && m_station.HasEnergy();
			}
			return true;
		}
		return false;
	}
	
	override void OnStartAnimationLoop(ActionData action_data) {
		super.OnStartAnimationLoop(action_data);
		if (GetGame().IsServer() && m_station && m_station.HasFuel(m_liquid) && m_station.HasEnergy()) {
			m_station.SetWorking(true);
		}
	}
	
	override void OnEndAnimationLoop(ActionData action_data) {
		super.OnEndAnimationLoop(action_data);
		if (GetGame().IsServer()) {
			if (m_station) {
				m_station.SetWorking(false);
				auto groupManager = GetFuelStationManager();
				FuelStationGroup group = groupManager.FindStationForPump(m_station.GetPosition());
				if (group) {
					float remaining = group.GetFuel(m_liquid);
					if (remaining != -1) {
						groupManager.SyncStation(group.m_config.id, "state.fuels." + m_liquid + ".available" , "" + remaining, true);
					}
				}
			}
		}
	}
};

class ActionFillGasolineBottleBase: ActionFillAtStationBottleBase {
	void ActionFillGasolineBottleBase() {
		m_liquid = LIQUID_GASOLINE;
	}
}

class ActionFillDieselBottleBase: ActionFillAtStationBottleBase {
	void ActionFillDieselBottleBase() {
		m_liquid = LIQUID_DIESEL;
	}
}

class ActionFillAVGasBottleBase: ActionFillAtStationBottleBase {
	void ActionFillAVGasBottleBase() {
		m_liquid = IE_FC_LIQUID_AVGAS;
	}
}

modded class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	
	override void CreateActionComponent() {
		
		FuelControlSettings settings = GetFuelControlSettings();
		auto target = m_ActionData.m_Target.GetObject();
		if (target) {
			float targetRate = settings.liquid_transfer_rates.Get(target.GetType());
			if (!targetRate) {
				targetRate = UAQuantityConsumed.FUEL;
			}
		}

		float itemRate = settings.liquid_transfer_rates.Get(m_ActionData.m_MainItem.GetType());
		if (!itemRate) {
			itemRate = UAQuantityConsumed.FILL_LIQUID;
		}
		
		float transferRate;
		
		if (targetRate && targetRate < itemRate) {
			transferRate = targetRate;
		} else {
			transferRate = itemRate;
		}
		
		m_liquid_type = ActionFillBottleBase.Cast( m_ActionData.m_Action ).GetLiquidType( m_ActionData.m_Player, m_ActionData.m_Target, m_ActionData.m_MainItem );
		m_ActionData.m_ActionComponent = new CAContinuousFill(transferRate, m_liquid_type);
	}
};