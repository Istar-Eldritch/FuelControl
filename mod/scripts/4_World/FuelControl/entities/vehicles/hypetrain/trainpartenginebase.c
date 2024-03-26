
#ifdef HypeTrain
    modded class HypeTrain_LocomotiveBase {

        float m_fuelDebt = 0;
        float m_Autonomy = 0;

        override bool StartEngine(bool forceStart = false, bool skipStartup = false)
        {
            ReloadFCConfigs();
		    Print("Starting engine!");
            return super.StartEngine(forceStart, skipStartup);
        }

		override void SetActions()
		{
			super.SetActions();
	        AddAction(ActionSiphon);
		}

        override protected void UpdateFuel(float dt) {
			if (m_Autonomy == 0) {
				super.UpdateFuel(dt);
			} else {
	            if ((m_EngineState == HypeTrain_EngineState.OFF) || !HasLiquidQuantity())
	                return;
	
	            float rpm;
	            if (!rpm) {
	                rpm = m_GeneratorRpm / m_GeneratorRpmMax;
	            }
	            
	            if (rpm < 0.1) {
	                rpm = 0.1;
	            }
	            
	            float speed = Math.AbsFloat(m_Velocity); // speed in m/s
	            float ds = dt * speed; // distance traveled in dt in m
	            
	            // fuel consumed in ds
	            float consumedFuel = (ds * GetLiquidQuantityMax() / m_Autonomy / 1000 + m_fuelDebt) * (rpm + 0.5);
	
	            if (consumedFuel > 1e-3) {
	                AddLiquidQuantity(-consumedFuel);
	                m_fuelDebt = 0;
	            } else {
	                m_fuelDebt = consumedFuel;
	            }
	        
	            // Shut off engine when empty
	            if (!HasEngineFuel())
	                StopEngine();
			}
        }

        void ReloadFCConfigs() {
            FuelControlSettings settings = GetFuelControlSettings();
            auto type = GetType();
            auto vehicle_config = settings.vehicle_config.Get(type);
			int fuelType = GetLiquidType();
            if (vehicle_config) {
                m_Autonomy = vehicle_config.autonomy;
                fuelType = IE_FC_LiquidFromString(vehicle_config.fuel_type);
                if (fuelType == -1) {
                    CF_Log.Error("[FuelControl] Couldn't parse fuel type for " + type + " likely a misconfiguration");
                    fuelType = GetLiquidType();
                }
            } else {
                m_Autonomy = 0;
            }
			if (fuelType != GetLiquidType()) {
				SetLiquidType(fuelType);	
			}
	    }
	
		override bool CanFillWithLiquid(int liquidType, bool ignore_fullness_check = false)
		{
			if(IsFullLiquidQuantity() && !ignore_fullness_check) return false;
	
			if(m_LiquidQuantity > 0 && m_LiquidType != liquidType) return false;
	
			return true;
		}
	

	    override protected void SetInitialLiquids()
	    {
	        super.SetInitialLiquids();
			ReloadFCConfigs();
	    }
    }
#endif