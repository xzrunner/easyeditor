#include "ComboBoxWidget.h"
#include "DynamicInfo.h"

namespace ee
{

std::string ComboBoxWidget::GetValue() const
{
	int sel = m_ctrl->GetSelection();
	if (sel >= 0 && sel < m_choices.size()) {
	 	if (sel == m_default) {
	 		return DEFAULT_VAL;
	 	} else {
	 		if (sel < m_choices.size()) {
	 			return m_choices[sel].value;
	 		} else {
	 			return "";
	 		}
	 	}
	} else {
		return m_ctrl->GetValue().ToStdString();
	}
}

bool ComboBoxWidget::IsChanged() const
{
	int now_val = m_ctrl->GetSelection();
	return now_val != m_ori_val;
}

void ComboBoxWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer, const DynamicInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	m_ori_val = m_default;
	std::string value = info.QueryValue(GetKey());
	wxString choices[MAX_ITEMS];
	bool find = false;
	for (int i = 0, n = m_choices.size(); i < n; ++i) {
		if (m_choices[i].value == value) {
			find = true;
			m_ori_val = i;
		}
		choices[i] = m_choices[i].title;
	}

	m_ctrl = new wxComboBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_choices.size(), choices); 
	if (find) {
		m_ctrl->SetSelection(m_ori_val);
	} else {
		if (!value.empty()) {
			m_ctrl->SetValue(value);
		} else {
			m_ctrl->SetSelection(m_ori_val);
		}
	}

	sizer->Add(m_ctrl);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void ComboBoxWidget::Load(const Json::Value& value)
{
	m_choices.clear();

	if (!value["default"].isNull()) {
		m_default = value["default"].asInt();		
	} else {
		m_default = -1;
	}

	int idx = 0;
	Json::Value v = value["choices"][idx++];
	while (!v.isNull()) {
		Item item;
		item.value = v["value"].asString();
		item.title = v["title"].asString();
		m_choices.push_back(item);

		v = value["choices"][idx++];
	}
}

}