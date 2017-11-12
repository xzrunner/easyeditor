#ifndef _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_
#define _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_

#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace ee
{
class FontBlankPropertySetting : public SpritePropertySetting
{
public:
	FontBlankPropertySetting(EditPanelImpl* stage, const SprPtr& spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	static const wxChar* HORI_ALIGN_LABELS[];
	static const wxChar* VERT_ALIGN_LABELS[];

}; // FontBlankPropertySetting
}

#endif // _EASYEDITOR_FONT_BLANK_PROPERTY_SETTING_H_