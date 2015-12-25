#include "CheckBoxWidget.h"
#include "DynamicInfo.h"

namespace lr
{

std::string CheckBoxWidget::GetValue() const
{
	if (m_has_default && m_default == m_ctrl->IsChecked()) {
		return DEFAULT_VAL;
	} else {
		return m_ctrl->IsChecked() ? "true" : "false";
	}
}

void CheckBoxWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
									const DynamicInfo& info)
{
	bool default_bool = m_default;
	std::string value = info.QueryValue(GetKey());
	if (value == "true") {
		default_bool = true;
	} else if (value == "false") {
		default_bool = false;
	}

	m_ctrl = new wxCheckBox(parent, wxID_ANY, GetTitle());
	m_ctrl->SetValue(default_bool);

	top_sizer->Add(m_ctrl);
	top_sizer->AddSpacer(10);
}

void CheckBoxWidget::Load(const Json::Value& value)
{
	m_has_default = !value["default"].isNull();
	m_default = value["default"].asBool();
}

}