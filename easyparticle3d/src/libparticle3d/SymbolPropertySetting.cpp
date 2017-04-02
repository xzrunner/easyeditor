#include "SymbolPropertySetting.h"
#include "ParticleSystem.h"

#include <ps_3d.h>

#include <ee/panel_msg.h>

namespace eparticle3d
{

SymbolPropertySetting::SymbolPropertySetting(ParticleSystem* ps)
	: ee::SymbolPropertySetting(&ps->name, &m_tag)
	, m_ps(ps)
{
	m_type = "Particle3DSymbol";
}

void SymbolPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SymbolPropertySetting::OnPropertyGridChange(name, value);

	if (name == "Loop") {
		m_ps->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Local") {
		m_ps->SetLocal(wxANY_AS(value, bool));
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SymbolPropertySetting::UpdateProperties(pg);

	pg->GetProperty("Loop")->SetValue(m_ps->IsLoop());
	pg->GetProperty("Local")->SetValue(m_ps->IsLocal());
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SymbolPropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("State", wxPG_LABEL));

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, m_ps->IsLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, m_ps->IsLocal()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}