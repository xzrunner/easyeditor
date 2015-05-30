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

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	static const wxChar* HORI_ALIGN_LABELS[];
	static const wxChar* VERT_ALIGN_LABELS[];

}; // FontPropertySetting
}

#endif // FONT_PROPERTY_SETTING_H