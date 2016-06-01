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
		m_rect->SetName(wxANY_AS(value, wxString).ToStdString());
	}
	else if (name == wxT("X"))
	{
		sm::rect r = m_rect->GetRect();
		const float x = wxANY_AS(value, float);
		const float hw = r.Size().x * 0.5f;
		r.xmin = x - hw;
		r.xmax = x + hw;
		m_rect->SetRect(r);
	}
	else if (name == wxT("Y"))
	{
		sm::rect r = m_rect->GetRect();
		const float y = wxANY_AS(value, float);
		const float hh = r.Size().y * 0.5f;
		r.ymin = y - hh;
		r.ymax = y + hh;
		m_rect->SetRect(r);
	}
	else if (name == wxT("Half Width"))
	{
		sm::rect r = m_rect->GetRect();
		const float x = r.Center().x;
		const float hw = wxANY_AS(value, float);
		r.xmin = x - hw;
		r.xmax = x + hw;
		m_rect->SetRect(r);
	}
	else if (name == wxT("Half Height"))
	{
		sm::rect r = m_rect->GetRect();
		const float y = r.Center().y;
		const float hh = wxANY_AS(value, float);
		r.ymin = y - hh;
		r.ymax = y + hh;
		m_rect->SetRect(r);
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
	const sm::rect& r = m_rect->GetRect();
	pg->GetProperty(wxT("Name"))->SetValue(m_rect->GetName());
	pg->GetProperty(wxT("X"))->SetValue(r.Center().x);
	pg->GetProperty(wxT("Y"))->SetValue(r.Center().y);
	pg->GetProperty(wxT("Half Width"))->SetValue(r.Size().x * 0.5f);
	pg->GetProperty(wxT("Half Height"))->SetValue(r.Size().y * 0.5f);
}

void RectPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_rect->GetName()));

	const sm::rect& r = m_rect->GetRect();

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, r.Center().x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, r.Center().y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Half Width"), wxPG_LABEL, r.Size().x * 0.5f));
	pg->SetPropertyAttribute(wxT("Half Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Half Width"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Half Height"), wxPG_LABEL, r.Size().y * 0.5f));
	pg->SetPropertyAttribute(wxT("Half Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Half Height"), "Precision", 1);
}

}