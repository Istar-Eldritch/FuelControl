modded class CAContinuousQuantityLiquidTransfer: CAContinuousBase {
	override void Setup( ActionData action_data ) {
		auto unmodifiedQuantity = m_QuantityUsedPerSecond;
		super.Setup(action_data);
		m_QuantityUsedPerSecond = unmodifiedQuantity;
	}
}