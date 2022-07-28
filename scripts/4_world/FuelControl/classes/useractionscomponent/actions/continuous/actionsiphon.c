class ActionSiphonCB : ActionContinuousBaseCB {

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
		m_ActionData.m_ActionComponent = new CASiphon( transferRate, LIQUID_GASOLINE );
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
		
		FuelControlSettings config = GetFuelControlSettings();
		
		if(!config.settings.siphoning)
			return false;

		CarScript car = CarScript.Cast( target.GetObject() );
		if (!car || car.GetFluidFraction(CarFluid.FUEL) <= (100 - config.settings.siphoning_limit) / 100)
			return false;
		
		// Check this item can be used to put fuel inside
		if(!item || !Liquid.CanFillContainer(item, LIQUID_GASOLINE))
			return false;
		
		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		
		if ( car ) {
			for (int s = 0; s < selections.Count(); s++) {
				if ( selections[s] == car.GetActionCompNameFuel() ) {
					vector refillPointPos = car.GetRefillPointPosWS();
					float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
					float distanceFuel = car.GetActionDistanceFuel() * car.GetActionDistanceFuel();
					return dist < distanceFuel;
				}
			}
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data ) {
		if (super.ActionConditionContinue( action_data )) {
			// TODO check if the item is full.
			
			FuelControlSettings config = GetFuelControlSettings();
			Car car = Car.Cast(action_data.m_Target.GetObject());
			float siphoning_limit = (100 - config.settings.siphoning_limit) / 100;
			if(siphoning_limit < 0) {
				siphoning_limit = 0;
			}
			if (!car || car.GetFluidFraction(CarFluid.FUEL) <= siphoning_limit)
				return false;

			return true;
		}
		return false;
	}
};
