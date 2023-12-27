#ifdef JM_COT
class FCForm: JMFormBase
{
	protected ref FCAdminForm m_adminForm;

	private FCModule m_Module;
	
	void ~FCForm() {
		delete m_adminForm;
	}

	protected override bool SetModule( JMRenderableModuleBase mdl )
	{
		return Class.CastTo( m_Module, mdl );
	}
	
	override void OnInit() {
		if (!m_adminForm) {
			m_adminForm = new FCAdminForm(layoutRoot);
		}
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