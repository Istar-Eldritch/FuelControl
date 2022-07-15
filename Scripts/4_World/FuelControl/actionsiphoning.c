class CASiphon : CAContinuousFill {
	
	override void Setup(ActionData action_data) {
		super.Setup(action_data);
	}
	
	override void CalcAndSetQuantity(ActionData action_data) {
		m_SpentQuantity_total += m_SpentQuantity;

		if ( m_SpentUnits ) {
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		if (GetGame().IsServer()) {
			Car car = Car.Cast(action_data.m_Target.GetObject());
			car.Leak( CarFluid.FUEL, (m_SpentQuantity / 1000));
			Liquid.FillContainerEnviro(action_data.m_MainItem, m_liquid_type, m_SpentQuantity, false);
		}
		
		m_SpentQuantity = 0;

	}
};

class ActionSiphonCB : ActionContinuousBaseCB {

	override void CreateActionComponent() {
		m_ActionData.m_ActionComponent = new CASiphon( UAQuantityConsumed.FUEL, LIQUID_GASOLINE );
	}
};

class ActionSiphon : ActionContinuousBase {
		
	void ActionSiphon() {
		m_CallbackClass = ActionSiphonCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = true;
	}
	
	override void CreateConditionComponents() {
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	override string GetText() {
		return "Siphon Fuel";
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		
		FuelControlSettings settings = GetFuelControlSettings();
		
		if(!settings.siphoning)
			return false;

		Car car = Car.Cast( target.GetObject() );
		if (!car || car.GetFluidFraction(CarFluid.FUEL) <= 0.2)
			return false;
		
		// Check this item can be used to put fuel inside
		if(!item || !Liquid.CanFillContainer(item, LIQUID_GASOLINE))
			return false;
		
		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		CarScript carS = CarScript.Cast(car);
		
		if ( carS ) {
			for (int s = 0; s < selections.Count(); s++) {
				if ( selections[s] == carS.GetActionCompNameFuel() ) {
					vector refillPointPos = carS.GetRefillPointPosWS();
					float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
					float distanceFuel = carS.GetActionDistanceFuel() * carS.GetActionDistanceFuel();
					return dist < distanceFuel;
				}
			}
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			// TODO check if the item is full.
			
			Car car = Car.Cast(action_data.m_Target.GetObject());
			if (!car || car.GetFluidFraction(CarFluid.FUEL) <= 0.2)
				return false;

			return true;
		}
		return false;
	}
};
