
// Can't do unless I can detect HypeTrain is installed
// modded class HypeTrain_LocomotiveBase {

//     override protected void UpdateFuel(float dt) {
//         if ((m_EngineState == HypeTrain_EngineState.OFF) || !HasLiquidQuantity())
//             return;

//         // Consume fuel
//         AddLiquidQuantity(-((m_FuelConsumption * m_GeneratorRpm) / 60.0) * dt);

//         // Shut off engine when empty
//         if (!HasEngineFuel())
//             StopEngine();
		
// 		Print("Modded!");
//     }

// }