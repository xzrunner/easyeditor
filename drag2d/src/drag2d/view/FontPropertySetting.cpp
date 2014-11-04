#include "FontPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/FontSprite.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

const wxChar* FontPropertySetting::HORI_ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), wxT("auto"), NULL };
const wxChar* FontPropertySetting::VERT_ALIGN_LABELS[] = { 
	wxT("top"), wxT("bottom"), wxT("center"), wxT("auto"), NULL };

FontPropertySetting::FontPropertySetting(EditPanel* editPanel, FontSprite* sprite)
	: SpritePropertySetting(editPanel, sprite)
{
	m_type = wxT("Font");
}

void FontPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	SpritePropertySetting::onPropertyGridChange(name, value);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	if (name == wxT("Font")) {
		sprite->font = wxANY_AS(value, wxString);
	} else if (name == wxT("Edge")) {
		sprite->has_edge = wxANY_AS(value, bool);
		if (sprite->has_edge) {
			sprite->font = "E";
		} else {
			sprite->font = "e";
		}
	} else if (name == wxT("FontColor")) {
		wxColour col = wxANY_AS(value, wxColour);
		sprite->color.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	} else if (name == wxT("Align.Hori")) {
		sprite->align_hori = HoriAlignType(wxANY_AS(value, int));
	} else if (name == wxT("Align.Vert")) {
		sprite->align_vert = VertAlignType(wxANY_AS(value, int));
	} else if (name == wxT("FontSize")) {
		sprite->size = wxANY_AS(value, float);
	} else if (name == "LabelSize") {
		double w, h;
		splitString(value, &w, &h);
		sprite->width = w;
		sprite->height = h;
		sprite->buildBounding();
	} else if (name == wxT("LabelSize.Width")) {
		sprite->width = wxANY_AS(value, float);
		sprite->buildBounding();
	} else if (name == wxT("LabelSize.Height")) {
		sprite->height = wxANY_AS(value, float);
		sprite->buildBounding();
	} else if (name == wxT("Filename")) {
		std::string str = wxANY_AS(value, wxString);
		sprite->loadFont(str);
	} else if (name == wxT("TextContent")) {
		sprite->SetTextContent(wxANY_AS(value, wxString));
	} else if (name == wxT("TextID")) {
		std::string tid = wxANY_AS(value, wxString);
		sprite->SetTextID(tid);
	}
}

void FontPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	SpritePropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();
	pg->GetProperty(wxT("Font"))->Enable(bEnable);
	pg->GetProperty(wxT("Edge"))->Enable(bEnable);
	pg->GetProperty(wxT("FontColor"))->Enable(bEnable);
	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("Align.Hori"))->Enable(bEnable);
	pg->GetProperty(wxT("Align.Vert"))->Enable(bEnable);
	pg->GetProperty(wxT("FontSize"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelSize.Width"))->Enable(bEnable);
	pg->GetProperty(wxT("LabelSize.Height"))->Enable(bEnable);
	pg->GetProperty(wxT("Filename"))->Enable(bEnable);
	pg->GetProperty(wxT("TextContent"))->Enable(bEnable);
	pg->GetProperty(wxT("TextID"))->Enable(bEnable);
}

void FontPropertySetting::updateProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::updateProperties(pg);

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->GetProperty(wxT("Font"))->SetValue(sprite->font);

	pg->GetProperty(wxT("Edge"))->SetValue(sprite->has_edge);

	wxColour col = wxColour(sprite->color.r*255, sprite->color.g*255, sprite->color.b*255, sprite->color.a*255);
	pg->SetPropertyValueString(wxT("FontColor"), col.GetAsString());

	pg->GetProperty(wxT("Align.Hori"))->SetValue(HORI_ALIGN_LABELS[sprite->align_hori]);
	pg->GetProperty(wxT("Align.Vert"))->SetValue(VERT_ALIGN_LABELS[sprite->align_vert]);
	pg->GetProperty(wxT("FontSize"))->SetValue(sprite->size);
	pg->GetProperty(wxT("LabelSize.Width"))->SetValue(sprite->width);
	pg->GetProperty(wxT("LabelSize.Height"))->SetValue(sprite->height);
	pg->GetProperty(wxT("Filename"))->SetValue(sprite->filename);
	pg->GetProperty(wxT("TextContent"))->SetValue(sprite->GetTextContext());
	pg->GetProperty(wxT("TextID"))->SetValue(sprite->GetTextID());
}

void FontPropertySetting::initProperties(wxPropertyGrid* pg)
{
	SpritePropertySetting::initProperties(pg);

	pg->Append(new wxPropertyCategory("FONT", wxPG_LABEL));

	FontSprite* sprite = static_cast<FontSprite*>(m_sprite);
	pg->Append(new wxStringProperty(wxT("Font"), wxPG_LABEL, sprite->font));

	pg->Append(new wxBoolProperty(wxT("Edge"), wxPG_LABEL, sprite->has_edge));

	wxColour col = wxColour(sprite->addCol.r*255, sprite->addCol.g*255, sprite->addCol.b*255, sprite->addCol.a*255);
	pg->Append(new wxColourProperty(wxT("FontColor"), wxPG_LABEL, col));
	pg->SetPropertyAttribute("FontColor", "HasAlpha", true);

	wxPGProperty* alignProp = pg->Append(new wxStringProperty(wxT("Align"), wxPG_LABEL, wxT("<composed>")));
	alignProp->SetExpanded(false);

	wxEnumProperty* horiAlignProp = new wxEnumProperty(wxT("Hori"), wxPG_LABEL, HORI_ALIGN_LABELS);
	horiAlignProp->SetValue(HORI_ALIGN_LABELS[sprite->align_hori]);
	pg->AppendIn(alignProp, horiAlignProp);

	wxEnumProperty* vertAlignProp = new wxEnumProperty(wxT("Vert"), wxPG_LABEL, VERT_ALIGN_LABELS);
	vertAlignProp->SetValue(VERT_ALIGN_LABELS[sprite->align_vert]);
	pg->AppendIn(alignProp, vertAlignProp);

	pg->Append(new wxFloatProperty(wxT("FontSize"), wxPG_LABEL, sprite->size));

	wxPGProperty* sizeProp = pg->Append(new wxStringProperty(wxT("LabelSize"), wxPG_LABEL, wxT("<composed>")));
	sizeProp->SetExpanded(false);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Width"), wxPG_LABEL, sprite->width));
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Height"), wxPG_LABEL, sprite->height));

	pg->Append(new wxStringProperty(wxT("Filename"), wxPG_LABEL, sprite->filename));
	pg->Append(new wxStringProperty(wxT("TextContent"), wxPG_LABEL, sprite->GetTextContext()));
	pg->Append(new wxStringProperty(wxT("TextID"), wxPG_LABEL, sprite->GetTextID()));
}

}