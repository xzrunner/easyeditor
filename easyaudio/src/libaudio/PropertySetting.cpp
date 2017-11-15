#include "PropertySetting.h"
#include "Sprite.h"

#include <ee/EditPanel.h>
#include <ee/EditPanelImpl.h>

namespace eaudio
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr)
	: ee::SpritePropertySetting(edit_impl, spr)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "Audio";
}

PropertySetting::~PropertySetting()
{
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
	if (name == "AudioOffset") {
		spr->SetAudioOffset(wxANY_AS(value, float));
	} else if (name == "Duration") {
		spr->SetAudioDuration(wxANY_AS(value, float));
	} else if (name == "FadeIn") {
		spr->SetFadeIn(wxANY_AS(value, float));
	} else if (name == "FadeOut") {
		spr->SetFadeOut(wxANY_AS(value, float));
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());

	pg->GetProperty("AudioOffset")->SetValue(spr->GetAudioOffset());
	pg->GetProperty("Duration")->SetValue(spr->GetAudioDuration());

	pg->GetProperty("FadeIn")->SetValue(spr->GetFadeIn());
	pg->GetProperty("FadeOut")->SetValue(spr->GetFadeOut());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("AUDIO", wxPG_LABEL));

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());

	pg->Append(new wxFloatProperty("AudioOffset", wxPG_LABEL, spr->GetAudioOffset()));
	pg->Append(new wxFloatProperty("Duration", wxPG_LABEL, spr->GetAudioDuration()));

	pg->Append(new wxFloatProperty("FadeIn", wxPG_LABEL, spr->GetFadeIn()));
	pg->Append(new wxFloatProperty("FadeOut", wxPG_LABEL, spr->GetFadeOut()));
}

}