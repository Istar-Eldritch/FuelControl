
class FCAdminListItemForm: ScriptedWidgetEventHandler {
	ref Widget layoutRoot;
	protected ref ButtonWidget m_delete;
	ref array<ref EditBoxWidget> m_edit_boxes;
	ref array<string> m_o;
	protected ref FCAdminListForm m_handler;
	int m_id;
	
	void FCAdminListItemForm(Widget parent, FCAdminListForm handler, int id, array<string> o, int columns) {
		m_o = o;
		m_id = id;
		m_handler = handler;
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("FuelControl/data/layouts/fc_admin_list_item" + columns + ".layout", parent);
		layoutRoot.SetHandler(this);
		m_edit_boxes = new array<ref EditBoxWidget>;
		for (int i = 0; i < columns; i++) {
			auto box = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("column" + i));
			box.SetText(o.Get(i));
			m_edit_boxes.Insert(box);
		}
		
		m_delete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("del_btn"));
	}
	
	string Get(int column) {
		return m_edit_boxes.Get(column).GetText();
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_delete) {
			m_handler.OnDelete(this);
			return true;
		}
        return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		m_handler.OnUpdate(this);
		return false;
	}
}

class FCAdminListForm: ScriptedWidgetEventHandler {
	
	ref Widget layoutRoot;
	protected ref Widget m_scroll;
	protected ref Widget m_content;
	protected ref ButtonWidget m_submit;
	protected ref ButtonWidget m_new;
	protected ref array<ref array<string>> m_data;
	protected ref array<ref FCAdminListItemForm> m_children;
	protected int m_columns;
	protected ref EditBoxWidget m_filterBox;
		
	protected ref array<string> m_new_item;
	
	void FCAdminListForm(Widget parent) {
		m_columns = GetColumns();
		int color = ARGB(255, 255, 255, 255);
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("FuelControl/data/layouts/fc_admin_list.layout", parent);
		layoutRoot.SetHandler(this);
		m_content = layoutRoot.FindAnyWidget("content");
		m_scroll = layoutRoot.FindAnyWidget("scroll");
		int submit_flags = WidgetFlags.VISIBLE || WidgetFlags.CLIPCHILDREN || WidgetFlags.HEXACTSIZE || WidgetFlags.VEXACTSIZE|| WidgetFlags.HEXACTPOS || WidgetFlags.VEXACTPOS;
		m_submit = ButtonWidget.Cast(layoutRoot.FindAnyWidget("submit"));
		m_new = ButtonWidget.Cast(layoutRoot.FindAnyWidget("new_btn"));
		m_filterBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("filter"));
		m_children = new array<ref FCAdminListItemForm>;
		m_new_item = null;
		m_data = GetData();
		UpdateUI();
	}
	
	int GetColumns() {
		return 2;
	}
	
	void ~FCAdminListForm() {
		delete m_children;
		delete layoutRoot;
	}
	
	array<ref array<string>> GetData() {
		array<ref array<string>> data = new array<ref array<string>>;
		return data;
	}

	void SortColumns() {
		bool swapped = true;
		int counter = 0;
		int swaps = 0;
		while (swapped) {          
		     while (counter < m_data.Count() - 1) {
				auto current = m_data.Get(counter);
				auto next = m_data.Get(counter + 1);
		     	if (current[0] > next[0]) {
					m_data.Set(counter, next);
					m_data.Set(counter + 1, current);
	                swaps = swaps + 1;
				}
		        counter = counter + 1;
			}
		    if (swaps == 0) {
		         swapped = false;
		    } else {
		         swaps = 0;
		         counter = 0;
			}
		}
	}
	
	array<ref array<string>> FilterKV(array<ref array<string>> input) {
		string filter = m_filterBox.GetText();
		filter.ToLower();
		if (filter != "") {
			auto newArray = new array<ref array<string>>;
			foreach (auto kv: input) {
				string key = kv[0];
				key.ToLower();
				if (key.Contains(filter)) {
					newArray.Insert(kv);
				}
			}
			return newArray;
		} else {
			return input;
		}
	}
	
	

	void UpdateUI() {
		
		for (int i = (m_children.Count() - 1); i >= 0; i--) {
			auto ch = m_children.Get(i);
			m_content.RemoveChild(ch.layoutRoot);
			m_children.Remove(i);
		}

		float root_height;
		float root_width;
		layoutRoot.GetParent().GetScreenSize(root_width, root_height);
		layoutRoot.SetScreenSize(root_width, root_height - 10);
		layoutRoot.SetPos(10, 10);
		m_scroll.SetScreenSize(root_width, root_height - 100);
		m_scroll.SetPos(0, 50);

		float content_height = 0;
		
		SortColumns();
		auto data = FilterKV(m_data);
		
		int id = 0;
		if (m_new_item) {
			FCAdminListItemForm new_item = FCAdminListItemForm(m_content, this, id, m_new_item, m_columns);
			id++;
			new_item.layoutRoot.SetPos(0, 0);
			m_children.Insert(new_item);
			content_height = 30;
		}
		
		foreach (auto fckv: data) {
			FCAdminListItemForm container = new FCAdminListItemForm(m_content, this, id, fckv, m_columns);
			container.layoutRoot.SetPos(0, content_height);
			m_children.Insert(container);
			content_height = content_height + 30;
			id++;
		}

		m_content.SetScreenSize(root_width, content_height);
		m_content.SetPos(0, 0);
		m_content.Update();
	}
	
	void OnSubmit() {
		delete m_new_item;
		m_filterBox.SetText("");
		m_data = GetData();
		UpdateUI();
	}
	
	void OnDelete(FCAdminListItemForm item) {
		m_content.RemoveChild(item.layoutRoot);
		m_children.Remove(item.m_id);
		if (m_new_item && item.m_id == 0) {
			delete m_new_item;
		} else {
			m_data.Remove(item.m_id);
		}
		UpdateUI();
	}
	
	void OnUpdate(FCAdminListItemForm item) {
		if (m_new_item && item.m_id == 0) {
			for (int i = 0; i < m_columns; i++) {
				m_new_item[i] = item.Get(i);
			}
		} else {
			auto d = m_data.Get(item.m_id);
			for (int ii = 0; ii < m_columns; ii++) {
				d[ii] = item.Get(ii);
			}
			m_data.Set(item.m_id, d);
		}
	}
	
	void OnNew() {
		m_new_item = new array<string>();
		for (int i = 0; i < m_columns; i++) {
			m_new_item.Insert("--");
		}
		UpdateUI();
	}

	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_submit) {
			OnSubmit();
			return true;
		} else if (w == m_new) {
			OnNew();
			return true;
		}
        return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		if (w == m_filterBox) {
			UpdateUI();
		}
		return false;
	}
}

bool IE_CmpFloats(float first, float second) {
	return Math.AbsFloat(first - second) < 0.000001;
}

class FCAdminVehicleForm: FCAdminListForm {
	override int GetColumns() {
		return 3;
	}
	override void OnSubmit() {
		FuelControlSettings st = GetFuelControlSettings();
		map<string, ref IE_FC_VehicleConfig> values = new map<string, ref IE_FC_VehicleConfig>;
		if (m_new_item) {
			auto new_vehicle_conf = new IE_FC_VehicleConfig(m_new_item[1].ToFloat(), m_new_item[2]);
			values.Set(m_new_item[0], new_vehicle_conf);
			st.vehicle_config.Set(m_new_item[0], new_vehicle_conf);

			st.SyncSetting("vehicle_config." + m_new_item[0] + ".autonomy", m_new_item[1]);
			st.SyncSetting("vehicle_config." + m_new_item[0] + ".fuel_type", m_new_item[2]);
		}
		foreach (auto kv: m_data) {
			values.Set(kv[0], new IE_FC_VehicleConfig(kv[1].ToFloat(), kv[2]));
		}
		
		foreach (auto key, auto value: st.vehicle_config) {
			auto newconf = values.Get(key);
			if (newconf) {
				if (newconf.fuel_type != value.fuel_type) {
					value.fuel_type = newconf.fuel_type;
					st.SyncSetting("vehicle_config." + key + ".fuel_type", newconf.fuel_type);
				}
				
				if (!IE_CmpFloats(newconf.autonomy, value.autonomy)) {
					value.autonomy = newconf.autonomy;
					st.SyncSetting("vehicle_config." + key + ".autonomy", "" + newconf.autonomy);
				}
			} else {
				st.vehicle_config.Remove(key);
				st.SyncRemoveSetting("vehicle_config." + key);
			}
		}
		super.OnSubmit();
	}
	
	override array<ref array<string>> GetData() {
		array<ref array<string>> data = new array<ref array<string>>;
		auto config = GetFuelControlSettings().vehicle_config;
		foreach (auto key, auto value: config) {
			auto fckv = new array<string>();
			fckv.Insert(key);
			fckv.Insert("" + value.autonomy);
			fckv.Insert("" + value.fuel_type);
			data.Insert(fckv);
		}
		return data;
	}
}

class FCAdminTransferRatesForm: FCAdminListForm {
	override int GetColumns() {
		return 2;
	}
	override void OnSubmit() {
		FuelControlSettings st = GetFuelControlSettings();
		map<string, float> values = new map<string, float>;
		if (m_new_item) {
			values.Set(m_new_item[0], m_new_item[1].ToFloat());
			st.liquid_transfer_rates.Set(m_new_item[0], m_new_item[1].ToFloat());
			st.SyncSetting("liquid_transfer." + m_new_item[0], m_new_item[1]);
		}
		foreach (auto kv: m_data) {
			float new_liquid_transfer = kv[1].ToFloat();
			values.Set(kv[0], new_liquid_transfer);
		}
		
		foreach (auto key, auto liquid_transfer: st.liquid_transfer_rates) {
			auto new_transfer = values.Get(key);
			if (new_transfer) {
				if (!IE_CmpFloats(liquid_transfer, new_transfer)) {
					st.liquid_transfer_rates.Set(key, new_transfer);
					st.SyncSetting("liquid_transfer." + key, "" + new_transfer);
				}
			} else {
				st.liquid_transfer_rates.Remove(key);
				st.SyncRemoveSetting("liquid_transfer." + key);
			}
		}
		super.OnSubmit();
	}
	
	override array<ref array<string>> GetData() {
		array<ref array<string>> data = new array<ref array<string>>;
		foreach (auto key, auto value: GetFuelControlSettings().liquid_transfer_rates) {
			auto fckv = new array<string>();
			fckv.Insert(key);
			fckv.Insert("" + value);
			data.Insert(fckv);
		}
		return data;
	}
}

class FCAdminPowerBoxForm: FCAdminListForm {
	override int GetColumns() {
		return 4;
	}
	override void OnSubmit() {
		FuelControlSettings st = GetFuelControlSettings();
		map<string, ref IE_FC_PowerBoxConfig> values = new map<string, ref IE_FC_PowerBoxConfig>;
		if (m_new_item) {
			auto id = FuelStationManager.GenId(m_new_item[0]);
			auto config = new IE_FC_PowerBoxConfig(id, m_new_item[1].ToFloat(), m_new_item[2].ToFloat(), m_new_item[3].ToFloat(), m_new_item[0]);
			values.Set(id, config);
			st.power_boxes.Insert(config);
			st.SyncSetting("powerbox." + config.id + ".x", "" + config.x);
			st.SyncSetting("powerbox." + config.id + ".y", "" + config.y);
			st.SyncSetting("powerbox." + config.id + ".orientation", "" + config.orientation);
			st.SyncSetting("powerbox." + config.id + ".name", "" + config.name);
		}
		foreach (auto kv: m_data) {
			auto dd = kv[4];
			auto conf = new IE_FC_PowerBoxConfig(dd, kv[1].ToFloat(), kv[2].ToFloat(), kv[3].ToFloat(), kv[0]);
			values.Set(dd, conf);
		}
		for(int i = (st.power_boxes.Count() - 1); i >= 0; i--) {
			auto powerbox = st.power_boxes[i];
			auto updated = values.Get(powerbox.id);
			
			if (updated) {
				if (IE_CmpFloats(updated.x, powerbox.x)) {
					powerbox.x = updated.x;
					st.SyncSetting("powerbox." + powerbox.id + ".x", "" + powerbox.x);
				}
				if (IE_CmpFloats(updated.y, powerbox.y)) {
					powerbox.y = updated.y;
					st.SyncSetting("powerbox." + powerbox.id + ".y", "" + powerbox.y);
				}
				if (IE_CmpFloats(updated.orientation, powerbox.orientation)) {
					powerbox.orientation = updated.orientation;
					st.SyncSetting("powerbox." + powerbox.id + ".orientation", "" + powerbox.orientation);
				}
				if (updated.name != powerbox.name) {
					powerbox.name = updated.name;
					st.SyncSetting("powerbox." + powerbox.id + ".name", powerbox.name);
				}
			} else {
				st.power_boxes.Remove(i);
				st.SyncRemoveSetting("powerbox." + powerbox.id);
			}
		}
		super.OnSubmit();
	}
	
	override void OnNew() {
		m_new_item = new array<string>();
		m_new_item.Insert("--");
		auto position = GetGame().GetPlayer().GetWorldPosition();
		m_new_item.Insert("" + position[0]);
		m_new_item.Insert("" + position[2]);
		auto orientation = GetGame().GetPlayer().GetOrientation();
		m_new_item.Insert("" + orientation[0]);
		UpdateUI();
	}
	
	override array<ref array<string>> GetData() {
		array<ref array<string>> data = new array<ref array<string>>;
		foreach (auto box: GetFuelControlSettings().power_boxes) {
			auto d = new array<string>();
			d.Insert(box.name);
			d.Insert("" + box.x);
			d.Insert("" + box.y);
			d.Insert("" + box.orientation);
			d.Insert(box.id);
			data.Insert(d);
		}
		return data;
	}
}