#include "FramePropertySetting.h"

#include "dataset/KeyFrame.h"

namespace eanim
{

FramePropertySetting::FramePropertySetting(d2d::EditPanel* editpanel, KeyFrame* frame)
	: d2d::IPropertySetting(editpanel, wxT("Frame"))
	, m_frame(frame)
{
}

void FramePropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();
	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("ID"))->SetValue(m_frame->m_id);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxFloatProperty(wxT("ID"), wxPG_LABEL, m_frame->m_id));
	}
}

void FramePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("ID"))
		m_frame->m_id = wxANY_AS(value, float);
}

void FramePropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void FramePropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxFloatProperty(wxT("ID"), wxPG_LABEL, m_frame->m_id));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("ID"))->Enable(bEnable);
}

} // eanim