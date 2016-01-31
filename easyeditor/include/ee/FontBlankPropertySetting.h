#ifndef _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_
#define _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_

#include "SpritePropertySetting.h"

namespace ee
{
class FontBlankSprite;

class FontBlankPropertySetting : public SpritePropertySetting
{
public:
	FontBlankPropertySetting(EditPanelImpl* stage, FontBlankSprite* sprite);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	static const wxChar* HORI_ALIGN_LABELS[];
	static const wxChar* VERT_ALIGN_LABELS[];

}; // FontBlankPropertySetting
}

#endif // _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_