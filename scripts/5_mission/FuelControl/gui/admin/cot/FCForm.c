#ifdef JM_COT
class FCForm: JMFormBase
{
	protected ref FCAdminStationForm m_stationForm;

	private FCModule m_Module;

	protected override bool SetModule( JMRenderableModuleBase mdl )
	{
		return Class.CastTo( m_Module, mdl );
	}
	
	override void OnInit() {
		m_stationForm = new FCAdminStationForm;
		layoutRoot.AddChild(m_stationForm.Init());
	}

	override void OnShow()
	{
		super.OnShow();
	}

	override void OnHide()
	{
		super.OnHide();
	}

	override void OnSettingsUpdated()
	{		
		// Update everything settings related here
	}
};
#endif