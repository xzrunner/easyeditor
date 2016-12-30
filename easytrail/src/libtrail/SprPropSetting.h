#ifndef _EASYTRAIL_SPR_PROP_SETTING_H_
#define _EASYTRAIL_SPR_PROP_SETTING_H_

#include <ee/SpritePropertySetting.h>

namespace etrail
{

class Sprite;

class SprPropSetting : public ee::SpritePropertySetting
{
public:
	SprPropSetting(ee::EditPanelImpl* edit_impl, Sprite* spr);
	virtual ~SprPropSetting();

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	wxWindow* m_parent;

	wxPropertyGrid* m_pg;

}; // SprPropSetting


}

#endif // _EASYTRAIL_SPR_PROP_SETTING_H_