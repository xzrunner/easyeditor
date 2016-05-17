#include "RectPropertySetting.h"
#include "RectShape.h"

#include <ee/panel_msg.h>

namespace eshape
{

RectPropertySetting::RectPropertySetting(ee::EditPanelImpl* stage, 
										 RectShape* rect)
	: ee::PropertySetting("Rect")
	, m_stage(stage)
	, m_rect(rect)
{
}

void RectPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_rect->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float hw = m_rect->m_rect.Size().x * 0.5f;
		m_rect->m_rect.xmin = x - hw;
		m_rect->m_rect.xmax = x + hw;
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float hh = m_rect->m_rect.Size().y * 0.5f;
		m_rect->m_rect.ymin = y - hh;
		m_rect->m_rect.ymax = y + hh;
	}
	else if (name == wxT("Half Width"))
	{
		const float x = m_rect->m_rect.Center().x;
		const float hw = wxANY_AS(value, float);
		m_rect->m_rect.xmin = x - hw;
		m_rect->m_rect.xmax = x + hw;
	}
	else if (name == wxT("Half Height"))
	{
		const float y = m_rect->m_rect.Center().y;
		const float hh = wxANY_AS(value, float);
		m_rect->m_rect.ymin = y - hh;
		m_rect->m_rect.ymax = y + hh;
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void RectPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_rect->name);
	pg->GetProperty(wxT("X"))->SetValue(m_rect->m_rect.Center().x);
	pg->GetProperty(wxT("Y"))->SetValue(m_rect->m_rect.Center().y);
	pg->GetProperty(wxT("Half Width"))->SetValue(m_rect->m_rect.Size().x * 0.5f);
	pg->GetProperty(wxT("Half Height"))->SetValue(m_rect->m_rect.Size().y * 0.5f);
}

void RectPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_rect->name));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_rect->m_rect.Center().x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_rect->m_rect.Center().y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Half Width"), wxPG_LABEL, m_rect->m_rect.Size().x * 0.5f));
	pg->SetPropertyAttribute(wxT("Half Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Half Width"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Half Height"), wxPG_LABEL, m_rect->m_rect.Size().y * 0.5f));
	pg->SetPropertyAttribute(wxT("Half Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Half Height"), "Precision", 1);
}

}