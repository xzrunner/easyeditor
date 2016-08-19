#include "PropertySetting.h"
#include "Sprite.h"
#include "ColorConfig.h"

#include <ee/EditPanelImpl.h>
#include <ee/EditPanel.h>
#include <ee/panel_msg.h>
#include <ee/Config.h>
#include <ee/SysColorProperty.h>
#include <ee/PropertyColorMonitor.h>

#include <wx/propgrid/advprops.h>

namespace etext
{

const wxChar* HORI_ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), wxT("auto"), wxT("tile"), NULL };
const wxChar* VERT_ALIGN_LABELS[] = { 
	wxT("top"), wxT("bottom"), wxT("center"), wxT("auto"), wxT("tile"), NULL };

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "Text";

	ColorConfig::Instance()->LoadFromFile();
}

PropertySetting::~PropertySetting()
{
	ColorConfig::Instance()->StoreToFile();
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	s2::Textbox& tb = spr->GetTextbox();
	if (name == "LabelSize") {
		double w, h;
		SplitString2Double(value, &w, &h);
		tb.width = w;
		tb.height = h;
		spr->UpdateBounding();
	} else if (name == "LabelSize.Width") {
		tb.width = wxANY_AS(value, int);
		spr->UpdateBounding();
	} else if (name == "LabelSize.Height") {
		tb.height = wxANY_AS(value, int);
		spr->UpdateBounding();
	} else if (name == "Font") {
		tb.font_type = wxANY_AS(value, int);
	} else if (name == "FontSize") {
		tb.font_size = wxANY_AS(value, int);
	} else if (name == "FontColor") {
		wxColour col = wxANY_AS(value, wxColour);
		tb.font_color = s2::Color(col.Red(), col.Green(), col.Blue(), col.Alpha());
	} else if (name == "Edge") {
		tb.has_edge = wxANY_AS(value, bool);
	} else if (name == "EdgeSize") {
		tb.edge_size = wxANY_AS(value, float);
	} else if (name == "EdgeColor") {
		wxColour col = wxANY_AS(value, wxColour);
		tb.edge_color = s2::Color(col.Red(), col.Green(), col.Blue(), col.Alpha());
	} else if (name == "Align.Hori") {
		tb.align_hori = s2::Textbox::HoriAlign(wxANY_AS(value, int));
	} else if (name == "Align.Vert") {
		tb.align_vert = s2::Textbox::VertAlign(wxANY_AS(value, int));
	} else if (name == "Overflow") {
		tb.overflow = wxANY_AS(value, bool);
	} else if (name == "Richtext") {
		tb.richtext = wxANY_AS(value, bool);
	} else if (name == "TextContent") {
		spr->SetText(wxANY_AS(value, wxString).ToStdString());
	} else if (name == "TextID") {
		spr->SetTID(wxANY_AS(value, wxString).ToStdString());
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	const s2::Textbox& tb = spr->GetTextbox();

	pg->GetProperty("LabelSize.Width")->SetValue(tb.width);
	pg->GetProperty("LabelSize.Height")->SetValue(tb.height);

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = ee::Config::Instance()->GetFonts();
	const std::vector<std::pair<std::string, std::string> >& 
		user_fonts = ee::Config::Instance()->GetUserFonts();
	wxArrayString choices;
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		choices.push_back(fonts[i].first);
	}
	for (int i = 0, n = user_fonts.size(); i < n; ++i) {
		choices.push_back(user_fonts[i].first);
	}
	pg->GetProperty("Font")->SetValue(choices[tb.font_type]);
	pg->GetProperty("FontSize")->SetValue(tb.font_size);
	const s2::Color& font_col = tb.font_color;
	pg->SetPropertyValueString("FontColor", wxColour(font_col.r, font_col.g, font_col.b, font_col.a).GetAsString());

	pg->GetProperty("Edge")->SetValue(tb.has_edge);
	pg->GetProperty("EdgeSize")->SetValue(tb.edge_size);
	const s2::Color& edge_col = tb.edge_color;
	pg->SetPropertyValueString("EdgeColor", wxColour(edge_col.r, edge_col.g, edge_col.b, edge_col.a).GetAsString());	

	pg->GetProperty("Align.Hori")->SetValue(HORI_ALIGN_LABELS[tb.align_hori]);
	pg->GetProperty("Align.Vert")->SetValue(VERT_ALIGN_LABELS[tb.align_vert]);

	pg->GetProperty("Overflow")->SetValue(tb.overflow);

	pg->GetProperty("Richtext")->SetValue(tb.richtext);

	pg->GetProperty("TextContent")->SetValue(spr->GetText());
	pg->GetProperty("TextID")->SetValue(spr->GetTID());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("TEXT", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	const s2::Textbox& tb = spr->GetTextbox();

	wxPGProperty* sz_prop = pg->Append(new wxStringProperty("LabelSize", wxPG_LABEL, "<composed>"));
	sz_prop->SetExpanded(false);
	pg->AppendIn(sz_prop, new wxIntProperty("Width", wxPG_LABEL, tb.width));
	pg->AppendIn(sz_prop, new wxIntProperty("Height", wxPG_LABEL, tb.height));

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = ee::Config::Instance()->GetFonts();
	const std::vector<std::pair<std::string, std::string> >& 
		user_fonts = ee::Config::Instance()->GetUserFonts();
	wxArrayString choices;
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		choices.push_back(fonts[i].first);
	}
	for (int i = 0, n = user_fonts.size(); i < n; ++i) {
		choices.push_back(user_fonts[i].first);
	}
	pg->Append(new wxEnumProperty("Font", wxPG_LABEL, choices));
	pg->Append(new wxIntProperty("FontSize", wxPG_LABEL, tb.font_size));

//	const s2::Color& font_col = spr->GetFontColor();
// 	pg->Append(new wxColourProperty("FontColor", wxPG_LABEL, wxColour(font_col.r, font_col.g, font_col.b, font_col.a)));
// 	pg->SetPropertyAttribute("FontColor", "HasAlpha", false);

	ee::SysColorProperty* col_prop = new ee::SysColorProperty("FontColor");
	col_prop->SetParent(m_parent);
	col_prop->SetColorData(ColorConfig::Instance()->GetColorData());
	col_prop->SetListener(new ee::PropertyColorListener(const_cast<s2::Color*>(&tb.font_color)));
	pg->Append(col_prop);

	pg->Append(new wxBoolProperty("Edge", wxPG_LABEL, tb.has_edge));
	pg->SetPropertyAttribute("Edge", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxFloatProperty("EdgeSize", wxPG_LABEL, tb.edge_size));
	const s2::Color& edge_col = tb.edge_color;
	pg->Append(new wxColourProperty("EdgeColor", wxPG_LABEL, wxColour(edge_col.r, edge_col.g, edge_col.b, edge_col.a)));
	pg->SetPropertyAttribute("EdgeColor", "HasAlpha", false);

	wxPGProperty* align_prop = pg->Append(new wxStringProperty("Align", wxPG_LABEL, "<composed>"));
	align_prop->SetExpanded(false);

	wxEnumProperty* hori_align_prop = new wxEnumProperty("Hori", wxPG_LABEL, HORI_ALIGN_LABELS);
	hori_align_prop->SetValue(HORI_ALIGN_LABELS[tb.align_hori]);
	pg->AppendIn(align_prop, hori_align_prop);

	wxEnumProperty* vert_align_prop = new wxEnumProperty("Vert", wxPG_LABEL, VERT_ALIGN_LABELS);
	vert_align_prop->SetValue(VERT_ALIGN_LABELS[tb.align_vert]);
	pg->AppendIn(align_prop, vert_align_prop);

	pg->Append(new wxBoolProperty("Overflow", wxPG_LABEL, tb.overflow));
	pg->SetPropertyAttribute("Overflow", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty("Richtext", wxPG_LABEL, tb.richtext));
	pg->SetPropertyAttribute("Richtext", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxStringProperty("TextContent", wxPG_LABEL, spr->GetText()));
	pg->Append(new wxStringProperty("TextID", wxPG_LABEL, spr->GetTID()));
}

}