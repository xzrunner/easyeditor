#ifndef _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_
#define _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace eparticle3d
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr);
	virtual ~SpritePropertySetting();

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	wxWindow* m_parent;

	wxPropertyGrid* m_pg;

}; // SpritePropertySetting


}

#endif // _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_