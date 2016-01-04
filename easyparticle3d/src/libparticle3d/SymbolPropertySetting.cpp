#include "SymbolPropertySetting.h"
#include "ParticleSystem.h"

#include <ps_3d.h>

namespace eparticle3d
{

SymbolPropertySetting::SymbolPropertySetting(ParticleSystem* ps)
	: d2d::SymbolPropertySetting(&ps->name, &m_tag)
	, m_ps(ps)
{
	m_type = "Particle3DSymbol";
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SymbolPropertySetting::OnPropertyGridChange(name, value);

	if (name == "Loop") {
		m_ps->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Local") {
		m_ps->SetLocalModeDraw(wxANY_AS(value, bool));
	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::UpdateProperties(pg);

	pg->GetProperty("Loop")->SetValue(m_ps->GetEmitter()->loop);
	pg->GetProperty("Local")->SetValue(m_ps->GetEmitter()->local_mode_draw);
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("State", wxPG_LABEL));

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, m_ps->GetEmitter()->loop));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, m_ps->GetEmitter()->local_mode_draw));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}