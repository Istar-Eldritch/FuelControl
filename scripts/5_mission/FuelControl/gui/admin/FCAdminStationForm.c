class FCAdminStationFormSubscriber: StationSubscriber {
	
	FCAdminStationForm m_form;
	bool m_pending_update;
	
	void FCAdminStationFormSubscriber(FCAdminStationForm form) {
		m_form = form;
		m_pending_update = false;
	}
	
	override void OnUpdate(FuelStationGroup station) {
		if (!m_pending_update) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateUI, 500);
			m_pending_update = true;
		}
	}
	
	void UpdateUI() {
		m_pending_update = false;
		m_form.UpdateUI();
	}
}

class FCAdminStationForm: UIScriptedMenu {
	
	protected ref ScrollWidget m_stationList;
	protected ref MapWidget m_stationMap;
	
	ref FuelStationManager m_stationManager;
	ref FCAdminStationFormSubscriber m_subscriber;
	
	void FCAdminStationForm() {
		m_stationManager = GetFuelStationManager();
		m_subscriber = FCAdminStationFormSubscriber(this);
		m_stationManager.Subscribe(m_subscriber);
	}
	
	void ~FCAdminStationForm() {
		m_stationManager.DeSubscribe(m_subscriber);
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form.layout");
		m_stationList = layoutRoot.FindAnyWidget("station_list");
		m_stationMap = layoutRoot.FindAnyWidget("station_map");
		m_stationManager.SendRequestAllStations();
		return layoutRoot;
	}

	void UpdateUI() {
		auto existing = m_stationList.GetChildren();
		if (existing) {
			m_stationList.RemoveChild(existing);
		}

		int counter = 0;
		foreach(auto station: m_stationManager.stations){
			if (station) {
				FCAdminStationFormListItem item = new FCAdminStationFormListItem(station);
				Widget item_widget = item.Init();
				float _;
				float item_height;
				item_widget.GetSize(_, item_height);
				item_widget.SetPos(0, item_height * counter);
				m_stationList.AddChild(item_widget);
				counter = counter + 1;
			}
		}
	}
}