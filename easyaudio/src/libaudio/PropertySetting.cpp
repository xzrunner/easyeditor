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
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("AUDIO", wxPG_LABEL));

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
}

}