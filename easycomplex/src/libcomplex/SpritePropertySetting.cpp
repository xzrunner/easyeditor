#include "SpritePropertySetting.h"
#include "Sprite.h"
#include "Symbol.h"

#include <ee/panel_msg.h>

namespace ecomplex
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
{
	m_type = "Complex";
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Action") {
		spr->SetAction(wxANY_AS(value, int) - 1);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	const Symbol* sym = dynamic_cast<const Symbol*>(spr->GetSymbol());

	std::vector<std::string> actions;
	sym->GetActionNames(actions);

	wxArrayString choices;
	choices.push_back("All");
	for (int i = 0, n = actions.size(); i < n; ++i) {
		choices.push_back(actions[i]);
	}

	pg->GetProperty("Action")->SetValue(spr->GetAction() + 1);
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("COMPLEX", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	const Symbol* sym = dynamic_cast<const Symbol*>(spr->GetSymbol());

	std::vector<std::string> actions;
	sym->GetActionNames(actions);

	int selected = spr->GetAction();

	wxArrayString choices;
	choices.push_back("All");
	for (int i = 0, n = actions.size(); i < n; ++i) {
		choices.push_back(actions[i]);
	}
	
	pg->Append(new wxEnumProperty("Action", wxPG_LABEL, choices, wxArrayInt(), selected + 1));	
}

}