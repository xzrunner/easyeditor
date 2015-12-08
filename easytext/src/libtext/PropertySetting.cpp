#include "PropertySetting.h"
#include "Sprite.h"
#include "ColorConfig.h"

#include <wx/propgrid/advprops.h>

namespace etext
{

const wxChar* HORI_ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), wxT("auto"), NULL };
const wxChar* VERT_ALIGN_LABELS[] = { 
	wxT("top"), wxT("bottom"), wxT("center"), wxT("auto"), NULL };

PropertySetting::PropertySetting(d2d::EditPanelImpl* edit_impl, Sprite* sprite)
	: d2d::SpritePropertySetting(edit_impl, sprite)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "Text";

	ColorConfig::Instance()->LoadFromFile();
}

PropertySetting::~PropertySetting()
{
	ColorConfig::Instance()->StoreToFile();
}

void PropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "LabelSize") {
		double w, h;
		SplitString2Double(value, &w, &h);
		spr->SetSize(w, h);
		spr->BuildBounding();
	} else if (name == "LabelSize.Width") {
		int w, h;
		spr->GetSize(w, h);
		w = wxANY_AS(value, int);
		spr->SetSize(w, h);
		spr->BuildBounding();
	} else if (name == "LabelSize.Height") {
		int w, h;
		spr->GetSize(w, h);
		h = wxANY_AS(value, int);
		spr->SetSize(w, h);
		spr->BuildBounding();
	} else if (name == "Font") {
		spr->SetFont(wxANY_AS(value, int));
	} else if (name == "FontSize") {
		spr->SetFontSize(wxANY_AS(value, int));
	} else if (name == "FontColor") {
		wxColour col = wxANY_AS(value, wxColour);
		spr->SetFontColor(d2d::Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f));
	} else if (name == "Edge") {
		spr->SetEdge(wxANY_AS(value, bool));
	} else if (name == "EdgeSize") {
		spr->SetEdgeSize(wxANY_AS(value, float));
	} else if (name == "EdgeColor") {
		wxColour col = wxANY_AS(value, wxColour);
		spr->SetEdgeColor(d2d::Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f));		
	} else if (name == "Align.Hori") {
		int h, v;
		spr->GetAlign(h, v);
		h = wxANY_AS(value, int);
		spr->SetAlign(h, v);
	} else if (name == "Align.Vert") {
		int h, v;
		spr->GetAlign(h, v);
		v = wxANY_AS(value, int);
		spr->SetAlign(h, v);
	} else if (name == "TextContent") {
		spr->SetText(wxANY_AS(value, wxString).ToStdString());
	} else if (name == "TextID") {
		spr->SetTID(wxANY_AS(value, wxString).ToStdString());
	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	int width, height;
	spr->GetSize(width, height);
	pg->GetProperty("LabelSize.Width")->SetValue(width);
	pg->GetProperty("LabelSize.Height")->SetValue(height);

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = d2d::Config::Instance()->GetFonts();
	wxArrayString choices;
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		choices.push_back(fonts[i].first);
	}

	wxLogDebug("PropertySetting::UpdateProperties choices: %d, font: %d", choices.size(), spr->GetFont());

	pg->GetProperty("Font")->SetValue(choices[spr->GetFont()]);
	pg->GetProperty("FontSize")->SetValue(spr->GetFontSize());
	const d2d::Colorf& font_col = spr->GetFontColor();
	pg->SetPropertyValueString("FontColor", wxColour(font_col.r*255, font_col.g*255, font_col.b*255, font_col.a*255).GetAsString());

	pg->GetProperty("Edge")->SetValue(spr->GetEdge());
	pg->GetProperty("EdgeSize")->SetValue(spr->GetEdgeSize());
	const d2d::Colorf& edge_col = spr->GetEdgeColor();
	pg->SetPropertyValueString("EdgeColor", wxColour(edge_col.r*255, edge_col.g*255, edge_col.b*255, edge_col.a*255).GetAsString());	

	int halign, valign;
	spr->GetAlign(halign, valign);
	pg->GetProperty("Align.Hori")->SetValue(HORI_ALIGN_LABELS[halign]);
	pg->GetProperty("Align.Vert")->SetValue(VERT_ALIGN_LABELS[valign]);

	pg->GetProperty("TextContent")->SetValue(spr->GetText());
	pg->GetProperty("TextID")->SetValue(spr->GetTID());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("TEXT", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	wxPGProperty* sz_prop = pg->Append(new wxStringProperty("LabelSize", wxPG_LABEL, "<composed>"));
	sz_prop->SetExpanded(false);
	int width, height;
	spr->GetSize(width, height);
	pg->AppendIn(sz_prop, new wxIntProperty("Width", wxPG_LABEL, width));
	pg->AppendIn(sz_prop, new wxIntProperty("Height", wxPG_LABEL, height));

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = d2d::Config::Instance()->GetFonts();
	wxArrayString choices;
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		choices.push_back(fonts[i].first);
	}
	pg->Append(new wxEnumProperty("Font", wxPG_LABEL, choices));
	pg->Append(new wxIntProperty("FontSize", wxPG_LABEL, spr->GetFontSize()));

//	const d2d::Colorf& font_col = spr->GetFontColor();
// 	pg->Append(new wxColourProperty("FontColor", wxPG_LABEL, wxColour(font_col.r*255, font_col.g*255, font_col.b*255, font_col.a*255)));
// 	pg->SetPropertyAttribute("FontColor", "HasAlpha", false);

	d2d::SysColorProperty* col_prop = new d2d::SysColorProperty("FontColor");
	col_prop->SetParent(m_parent);
	col_prop->SetColorData(ColorConfig::Instance()->GetColorData());
	col_prop->SetListener(new d2d::PropertyColorListener(&spr->GetFontColor()));
	pg->Append(col_prop);

	pg->Append(new wxBoolProperty("Edge", wxPG_LABEL, spr->GetEdge()));
	pg->SetPropertyAttribute("Edge", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxFloatProperty("EdgeSize", wxPG_LABEL, spr->GetEdgeSize()));
	const d2d::Colorf& edge_col = spr->GetEdgeColor();
	pg->Append(new wxColourProperty("EdgeColor", wxPG_LABEL, wxColour(edge_col.r*255, edge_col.g*255, edge_col.b*255, edge_col.a*255)));
	pg->SetPropertyAttribute("EdgeColor", "HasAlpha", false);

	wxPGProperty* align_prop = pg->Append(new wxStringProperty("Align", wxPG_LABEL, "<composed>"));
	align_prop->SetExpanded(false);

	int halign, valign;
	spr->GetAlign(halign, valign);

	wxEnumProperty* hori_align_prop = new wxEnumProperty("Hori", wxPG_LABEL, HORI_ALIGN_LABELS);
	hori_align_prop->SetValue(HORI_ALIGN_LABELS[halign]);
	pg->AppendIn(align_prop, hori_align_prop);

	wxEnumProperty* vert_align_prop = new wxEnumProperty("Vert", wxPG_LABEL, VERT_ALIGN_LABELS);
	vert_align_prop->SetValue(VERT_ALIGN_LABELS[valign]);
	pg->AppendIn(align_prop, vert_align_prop);

	pg->Append(new wxStringProperty("TextContent", wxPG_LABEL, spr->GetText()));
	pg->Append(new wxStringProperty("TextID", wxPG_LABEL, spr->GetTID()));
}

}