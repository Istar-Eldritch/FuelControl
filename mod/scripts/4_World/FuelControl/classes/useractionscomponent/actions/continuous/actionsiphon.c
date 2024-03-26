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
		int liquid;
		CarScript car = CarScript.Cast( m_ActionData.m_Target.GetObject() );
		if (car) {
			liquid = car.GetFuelType();
		}
		
		#ifdef HypeTrain
			auto trainTarget = HypeTrain_PartBase.Cast(m_ActionData.m_Target.GetObject());
			if (trainTarget) {
				liquid = trainTarget.GetLiquidType();
			}
		#endif
			
		if (!liquid) {
			CF_Log.Error("[FuelControl] Couldn't get fuel type for vehicle for siphoning, likely a bug");
			liquid = LIQUID_GASOLINE;
		}
		m_ActionData.m_ActionComponent = new CASiphon( transferRate, liquid );
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
		string siphonLoc = Widget.TranslateString( "#STR_IEFC_SIPHON" );
		return siphonLoc;
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		
		FuelControlSettings config = GetFuelControlSettings();
		
		if(!config.settings.siphoning)
			return false;

		float siphoning_limit = (100 - config.settings.siphoning_limit) / 100;
		if(siphoning_limit < 0) {
			siphoning_limit = 0;
		}

		CarScript car = CarScript.Cast( target.GetObject() );
		if (car) {
			if (GetGame().IsServer()) {
				car.Synchronize();
			}

			if (car.GetFluidFraction(CarFluid.FUEL) <= siphoning_limit)
				return false;
			
			// Check this item can be used to put fuel inside
			if(!item || !Liquid.CanFillContainer(item, car.GetFuelType()))
				return false;
			
			array<string> selections = new array<string>;
			target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

			for (int s = 0; s < selections.Count(); s++) {
				if ( selections[s] == car.GetActionCompNameFuel() ) {
					vector refillPointPos = car.GetRefillPointPosWS();
					float dist = vector.DistanceSq(refillPointPos, player.GetPosition() );
					float distanceFuel = car.GetActionDistanceFuel() * car.GetActionDistanceFuel();
					return dist < distanceFuel;
				}
			}
		}

		#ifdef HypeTrain
			auto trainTarget = HypeTrain_PartBase.Cast(target.GetObject());
			if (trainTarget) {
				auto cmpName = target.GetObject().GetActionComponentName(target.GetComponentIndex());
				if(!cmpName.Contains("pour"))
					return false;

				float fraction = trainTarget.GetLiquidQuantity() / trainTarget.GetLiquidQuantityMax();

				if (fraction <= siphoning_limit)
					return false;

				return trainTarget.CanTransferLiquidToItem(item);
			}
		#endif

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
			if (car) {
				if (car.GetFluidFraction(CarFluid.FUEL) <= siphoning_limit)
					return false;

				return true;
			}

			#ifdef HypeTrain
				auto trainTarget = HypeTrain_PartBase.Cast(action_data.m_Target.GetObject());
				if (trainTarget) {
					float fraction = trainTarget.GetLiquidQuantity() / trainTarget.GetLiquidQuantityMax();

					if (fraction <= siphoning_limit)
						return false;

					return true;
				}
			#endif
		}
		return false;
	}
};
