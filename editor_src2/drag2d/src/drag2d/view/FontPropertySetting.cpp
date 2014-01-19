#include "FontPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/FontSprite.h"

namespace d2d
{
const wxChar* FontPropertySetting::ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), NULL };

FontPropertySetting::FontPropertySetting(EditPanel* editPanel, FontSprite* sprite)
	: SpritePropertySetting(editPanel, sprite)
{
	m_type = wxT("Font");
}

void FontPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	SpritePropertySetting::onPropertyGridChange(name, value);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	if (name == wxT("Font"))
		sprite->font = wxANY_AS(value, wxString);
	else if (name == wxT("Color"))
		sprite->color = wxANY_AS(value, wxString);
	else if (name == wxT("Align")) 
		sprite->align = AlignType(wxANY_AS(value, int));
	else if (name == wxT("Size"))
		sprite->size = wxANY_AS(value, float);
	else if (name == wxT("LabelWidth")) {
		sprite->width = wxANY_AS(value, float);
		sprite->buildBounding();
	}
	else if (name == wxT("LabelHeight")) {
		sprite->height = wxANY_AS(value, float);
		sprite->buildBounding();
	}
}

void FontPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	SpritePropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();
	pg->GetProperty(wxT("Font"))->Enable(bEnable);
	pg->GetProperty(wxT("Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("Size"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelWidth"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelHeight"))->Enable(bEnable);
}

void FontPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::updateProperties(pg);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->GetProperty(wxT("Font"))->SetValue(sprite->font);
	pg->GetProperty(wxT("Color"))->SetValue(sprite->color);
	pg->GetProperty(wxT("Align"))->SetValue(ALIGN_LABELS[sprite->align]);
	pg->GetProperty(wxT("Size"))->SetValue(sprite->size);
	pg->GetProperty(wxT("LabelWidth"))->SetValue(sprite->width);
	pg->GetProperty(wxT("LabelHeight"))->SetValue(sprite->height);
}

void FontPropertySetting::initProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::initProperties(pg);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->Append(new wxStringProperty(wxT("Font"), wxPG_LABEL, sprite->font));
	pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, sprite->color));
	pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, ALIGN_LABELS));
	pg->Append(new wxFloatProperty(wxT("Size"), wxPG_LABEL, sprite->size));
	pg->Append(new wxFloatProperty(wxT("LabelWidth"), wxPG_LABEL, sprite->width));
	pg->Append(new wxFloatProperty(wxT("LabelHeight"), wxPG_LABEL, sprite->height));
}

}