#ifdef JM_COT
class FCModule: JMRenderableModuleBase
{
	
	void FCModule() 
	{
		GetPermissionsManager().RegisterPermission( "Admin.FuelControl.View" );
        GetPermissionsManager().RegisterPermission( "Admin.FuelControl.Edit" );
	}
	
	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Admin.Map.View" );
	}
	
	override string GetLayoutRoot()
	{
		return "FuelControl/data/layouts/cot_form.layout";
	}

	override string GetTitle()
	{
		return "FuelControl";
	}
	
	override string GetIconName()
	{
		return "FC";
	}
	
	override bool ImageIsIcon()
	{
		return false;
	}
};
#endif