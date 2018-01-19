#pragma once

#include <ee/SpritePropertySetting.h>

namespace enode3d
{

class Sprite;

class PropertySetting2 : public ee::SpritePropertySetting
{
public:
	PropertySetting2(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

}; // PropertySetting2

}