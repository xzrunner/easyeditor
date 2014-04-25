#ifndef FONT_PROPERTY_SETTING_H
#define FONT_PROPERTY_SETTING_H

#include "SpritePropertySetting.h"

namespace d2d
{
class FontSprite;

class FontPropertySetting : public SpritePropertySetting
{
public:
	FontPropertySetting(EditPanel* editPanel, FontSprite* sprite);

	virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
	virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

protected:
	virtual void updateProperties(wxPropertyGrid* pg);
	virtual void initProperties(wxPropertyGrid* pg);

private:
	static const wxChar* HORI_ALIGN_LABELS[];
	static const wxChar* VERT_ALIGN_LABELS[];

}; // FontPropertySetting
}

#endif // FONT_PROPERTY_SETTING_H