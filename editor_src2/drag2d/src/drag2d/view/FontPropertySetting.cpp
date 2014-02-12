#include "FontPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/FontSprite.h"

#include <wx/propgrid/advprops.h>

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
	else if (name == wxT("Color")) {
		wxColour col = wxANY_AS(value, wxColour);
		sprite->color.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	}
	else if (name == wxT("Align")) 
		sprite->align = AlignType(wxANY_AS(value, int));
	else if (name == wxT("FontSize"))
		sprite->size = wxANY_AS(value, float);
	else if (name == wxT("LabelWidth")) {
		sprite->width = wxANY_AS(value, float);
		sprite->buildBounding();
	}
	else if (name == wxT("LabelHeight")) {
		sprite->height = wxANY_AS(value, float);
		sprite->buildBounding();
	} else if (name == wxT("Filename")) {
		std::string str = wxANY_AS(value, wxString);
		sprite->loadFont(str);
	}
}

void FontPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	SpritePropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();
	pg->GetProperty(wxT("Font"))->Enable(bEnable);
	pg->GetProperty(wxT("Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("FontSize"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelWidth"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelHeight"))->Enable(bEnable);
	pg->GetProperty(wxT("Filename"))->Enable(bEnable);
}

void FontPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::updateProperties(pg);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->GetProperty(wxT("Font"))->SetValue(sprite->font);

	wxColour col = wxColour(sprite->color.r*255, sprite->color.g*255, sprite->color.b*255, sprite->color.a*255);
	pg->SetPropertyValueString(wxT("Color"), col.GetAsString());

	pg->GetProperty(wxT("Align"))->SetValue(ALIGN_LABELS[sprite->align]);
	pg->GetProperty(wxT("FontSize"))->SetValue(sprite->size);
	pg->GetProperty(wxT("LabelWidth"))->SetValue(sprite->width);
	pg->GetProperty(wxT("LabelHeight"))->SetValue(sprite->height);
	pg->GetProperty(wxT("Filename"))->SetValue(sprite->filename);
}

void FontPropertySetting::initProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::initProperties(pg);

	pg->Append(new wxPropertyCategory("FONT", wxPG_LABEL));

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->Append(new wxStringProperty(wxT("Font"), wxPG_LABEL, sprite->font));

	wxColour col = wxColour(sprite->addCol.r*255, sprite->addCol.g*255, sprite->addCol.b*255, sprite->addCol.a*255);
	pg->Append(new wxColourProperty(wxT("Color"), wxPG_LABEL, col));
	pg->SetPropertyAttribute("Color", "HasAlpha", true);

	wxEnumProperty* alignProp = new wxEnumProperty(wxT("Align"), wxPG_LABEL, ALIGN_LABELS);
	alignProp->SetValue(ALIGN_LABELS[sprite->align]);
	pg->Append(alignProp);
	pg->Append(new wxFloatProperty(wxT("FontSize"), wxPG_LABEL, sprite->size));
	pg->Append(new wxFloatProperty(wxT("LabelWidth"), wxPG_LABEL, sprite->width));
	pg->Append(new wxFloatProperty(wxT("LabelHeight"), wxPG_LABEL, sprite->height));
	pg->Append(new wxStringProperty(wxT("Filename"), wxPG_LABEL, sprite->filename));
}

}