
class FCAdminStationFormListItem: ScriptedWidgetEventHandler {
	
	protected ref Widget m_infoPanel;
	protected ref TextWidget m_name_txt;
	protected ref TextWidget m_location_x_txt;
	protected ref TextWidget m_location_y_txt;
	protected ref TextWidget m_capacity_txt;
	protected ref TextWidget m_available_txt;
	protected ref ButtonWidget m_tp_btn;
	protected ref ButtonWidget m_edit_btn;
	protected ref ButtonWidget m_del_btn;
	
	protected ref Widget m_editPanel;
	protected ref EditBoxWidget m_name_edit;
	protected ref EditBoxWidget m_location_x_edit;
	protected ref EditBoxWidget m_location_y_edit;
	protected ref EditBoxWidget m_capacity_edit;
	protected ref EditBoxWidget m_available_edit;
	protected ref ButtonWidget m_ok_edit_btn;
	protected ref ButtonWidget m_cancel_edit_btn;
	
	protected ref Widget m_delPanel;
	protected ref TextWidget m_del_txt;
	protected ref ButtonWidget m_ok_del_btn;
	protected ref ButtonWidget m_cancel_del_btn;

	ref FuelStationGroup m_station;
	ref Widget layoutRoot;
	ref FCSelectStationSubscriber m_onSelectSubscriber;
	
	const int STATE_INFO = 0;
	const int STATE_EDIT = 1;
	const int STATE_DEL = 2;
	
	int m_currentState = STATE_INFO;
	
	void FCAdminStationFormListItem(Widget parent, FuelStationGroup station, FCSelectStationSubscriber onSelectSubscriber = null) {
		m_station = station;
		m_onSelectSubscriber = onSelectSubscriber;
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "FuelControl/GUI/layouts/fc_admin_station_form_list_item.layout", parent);
		layoutRoot.SetHandler(this);
		OnInit();
	}
	
	void ~FCAdminStationFormListItem() {
		delete layoutRoot;
	}
	
	void OnInit() {
		m_infoPanel = layoutRoot.FindAnyWidget("info_panel");
		m_name_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("name_txt"));
		m_capacity_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("capacity_txt"));
		m_available_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("available_txt"));
		m_location_x_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_x_txt"));
		m_location_y_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("location_y_txt"));
		m_tp_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("tp_btn"));
		m_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("edit_btn"));
		m_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("del_btn"));
		
		m_editPanel = layoutRoot.FindAnyWidget("edit_panel");
		m_name_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("name_edit"));
		m_capacity_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("capacity_edit"));
		m_available_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("available_edit"));
		m_location_x_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("location_x_edit"));
		m_location_y_edit = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("location_y_edit"));
		m_ok_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_edit_btn"));
		m_cancel_edit_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_edit_btn"));
		
		m_delPanel = layoutRoot.FindAnyWidget("del_panel");
		m_del_txt = TextWidget.Cast(layoutRoot.FindAnyWidget("del_txt"));
		m_ok_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_del_btn"));
		m_cancel_del_btn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_del_btn"));

		UpdateUI();
	}
	
	void _SetupInfoPanel() {
		m_infoPanel.Show(true);
		m_name_txt.SetText(m_station.name);

		string capacity;
        if (m_station.fuelCapacity >= 0) {
            auto scaledCapacity = m_station.fuelCapacity / 1000;
            capacity = "" + scaledCapacity + "L";
        } else {
            capacity = "Infinite";
        }
		m_capacity_txt.SetText(capacity);
		
		string available;
        if (m_station.fuelAmount >= 0) {
            auto scaledAmount = m_station.fuelAmount / 1000;
            available = "" + scaledAmount + "L";
        } else {
            available = "Infinite";
        }
		m_available_txt.SetText(available);
		
		m_location_x_txt.SetText("" + m_station.position[0]);
		m_location_y_txt.SetText("" + m_station.position[2]);
	}
	
	void _SetupEditPanel() {
		m_editPanel.Show(true);
		m_name_edit.SetText(m_station.name);
		
		string capacity;
        if (m_station.fuelCapacity >= 0) {
            capacity = "" + (m_station.fuelCapacity / 1000);
        } else {
            capacity = "" + -1;
        }
		m_capacity_edit.SetText(capacity);

		string available;
        if (m_station.fuelAmount >= 0) {
            available = "" + (m_station.fuelAmount / 1000);
        } else {
            available = "" + -1;
        }
		m_available_edit.SetText(available);

		m_location_x_edit.SetText("" + m_station.position[0]);
		m_location_y_edit.SetText("" + m_station.position[2]);
	}
	
	void _SetupDelPanel() {
		m_delPanel.Show(true);
		m_del_txt.SetText("Delete " + m_station.name + "?");
	}
	
	void UpdateUI() {
		m_infoPanel.Show(false);
		m_editPanel.Show(false);
		m_delPanel.Show(false);
		if (m_currentState == STATE_INFO) {
			_SetupInfoPanel();
		} else if (m_currentState == STATE_EDIT) {
			_SetupEditPanel();
		} else if (m_currentState == STATE_DEL) {
			_SetupDelPanel();
		}
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (button == 0) {
			if (w == m_tp_btn) {
				FCGetTeleportManager().SendTeleportToStation(m_station);
			} else if (w == m_edit_btn) {
				m_currentState = STATE_EDIT;
				UpdateUI();
			} else if (w == m_del_btn) {
				m_currentState = STATE_DEL;
				UpdateUI();
			} else if (w == m_cancel_edit_btn) {
				if (!m_station.id) {
					m_onSelectSubscriber.OnCreateCancel();
				}
				m_currentState = STATE_INFO;
				UpdateUI();
			} else if (w == m_ok_edit_btn) {
				m_station.name = m_name_edit.GetText();
				auto v = m_station.position;
				v[0] = m_location_x_edit.GetText().ToFloat();
				v[1] = 0;
				v[2] = m_location_y_edit.GetText().ToFloat();
				m_station.position = v;
				m_station.fuelCapacity = m_capacity_edit.GetText().ToFloat() * 1000;
				m_station.fuelAmount = m_available_edit.GetText().ToFloat() * 1000;
				bool isNew = false;
				if (!m_station.id) {
					m_station.id = FuelStationManager.GenId(m_station.name);
					isNew = true;
				}
				GetFuelStationManager().UpdateStation(m_station, true);
				if (isNew) {
					m_onSelectSubscriber.OnCreate(m_station);
				} else {
					m_currentState = STATE_INFO;
					UpdateUI();
				}
			} else if (w == m_cancel_del_btn) {
				m_currentState = STATE_INFO;
				UpdateUI();
			} else if (w == m_ok_del_btn) {
				GetFuelStationManager().DeleteStation(m_station, true);
			} else if (m_onSelectSubscriber) {
				m_onSelectSubscriber.OnSelect(m_station);
			}
			return true;
		} else if (button == 1) {
			m_onSelectSubscriber.OnOptions(m_station, x, y);
			return true;
		}
		return false;
	}
	
}