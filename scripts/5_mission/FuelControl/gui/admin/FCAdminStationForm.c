class FCAdminStationFormSubscriber: StationSubscriber {
	
	FCAdminStationForm m_form;
	bool m_pending_update;
	
	void FCAdminStationFormSubscriber(FCAdminStationForm form) {
		m_form = form;
		m_pending_update = false;
	}
	
	override void OnUpdate(FuelStationGroup station) {
		//if (!m_pending_update) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateUI, 500, false, station);
		//	m_pending_update = true;
		//}
	}
	
	void UpdateUI(FuelStationGroup station) {
		//m_pending_update = false;
		m_form.UpdateUI(station);
	}
}

class FCAdminStationForm: UIScriptedMenu {
	
	protected ref ScrollWidget m_stationScroll;
	protected ref Widget m_stationList;
	protected ref MapWidget m_stationMap;
	
	ref FuelStationManager m_stationManager;
	ref FCAdminStationFormSubscriber m_subscriber;
	
	ref map<string, FCAdminStationFormListItem> m_children;
	
	void FCAdminStationForm() {
		m_children = new map<string, FCAdminStationFormListItem>;
		m_stationManager = GetFuelStationManager();
		m_subscriber = FCAdminStationFormSubscriber(this);
		m_stationManager.Subscribe(m_subscriber);
	}
	
	void ~FCAdminStationForm() {
		m_stationManager.DeSubscribe(m_subscriber);
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form.layout");
		m_stationScroll = ScrollWidget.Cast(layoutRoot.FindAnyWidget("sation_scroll"));
		m_stationScroll.VScrollToPos(0);
		m_stationList = layoutRoot.FindAnyWidget("station_list");
		m_stationMap = MapWidget.Cast(layoutRoot.FindAnyWidget("station_map"));
		m_stationManager.SendRequestAllStations();
		return layoutRoot;
	}

	void UpdateUI(FuelStationGroup station) {
		if (station) {
			auto item = m_children.Get(station.name);
			if (item) {
				item.OnStationUpdate(station);
			} else {
				item = new FCAdminStationFormListItem(station);
				Widget item_widget = item.Init();
				float item_width;
				float item_height;
				item_widget.GetSize(item_width, item_height);
				item_widget.SetPos(0, item_height * m_children.Count());
				m_stationList.AddChild(item_widget);
				m_children.Insert(station.name, item);
				m_stationList.SetSize(item_width, item_height * m_children.Count());
			}
		}
	}
}