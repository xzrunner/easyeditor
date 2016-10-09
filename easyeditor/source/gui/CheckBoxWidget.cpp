#include "CheckBoxWidget.h"
#include "DynamicInfo.h"

namespace ee
{

std::string CheckBoxWidget::GetValue() const
{
	if (m_has_default && m_default == m_ctrl->IsChecked()) {
		return DEFAULT_VAL;
	} else {
		return m_ctrl->IsChecked() ? "true" : "false";
	}
}

bool CheckBoxWidget::IsChanged() const
{
	bool now_val = m_ctrl->IsChecked();
	return now_val != m_ori_val;
}

void CheckBoxWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer, const DynamicInfo& info)
{
	m_ori_val = m_default;
	std::string value = info.QueryValue(GetKey());
	if (value == "true") {
		m_ori_val = true;
	} else if (value == "false") {
		m_ori_val = false;
	}

	m_ctrl = new wxCheckBox(parent, wxID_ANY, GetTitle());
	m_ctrl->SetValue(m_ori_val);

	top_sizer->Add(m_ctrl);
	top_sizer->AddSpacer(10);
}

void CheckBoxWidget::Load(const Json::Value& value)
{
	m_has_default = !value["default"].isNull();
	m_default = value["default"].asBool();
}

}