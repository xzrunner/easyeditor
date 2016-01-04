#ifndef _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_
#define _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eparticle3d
{

class Sprite;

class SpritePropertySetting : public d2d::SpritePropertySetting
{
public:
	SpritePropertySetting(d2d::EditPanelImpl* edit_impl, Sprite* sprite);
	virtual ~SpritePropertySetting();

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	wxWindow* m_parent;

}; // SpritePropertySetting


}

#endif // _EASYPARTICLE3D_SPRITE_PROPERTY_SETTING_H_