
override class ActionPourLiquidTrain
{

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        auto trainTarget = HypeTrain_PartBase.Cast(target.GetObject());
        if (!trainTarget)
            return false;

        auto cmpName = target.GetObject().GetActionComponentName(target.GetComponentIndex());
        if(!cmpName.Contains("pour"))
            return false;

        return trainTarget.CanTransferLiquidFromItem(item);
    }
}
