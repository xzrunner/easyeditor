#include "PropertySetting.h"
#include "Sprite.h"

#include <wx/propgrid/advprops.h>

namespace etext
{

const wxChar* HORI_ALIGN_LABELS[] = { 
	wxT("left"), wxT("right"), wxT("center"), wxT("auto"), NULL };
const wxChar* VERT_ALIGN_LABELS[] = { 
	wxT("top"), wxT("bottom"), wxT("center"), wxT("auto"), NULL };

PropertySetting::PropertySetting(d2d::EditPanelImpl* edit_impl, Sprite* sprite)
	: d2d::SpritePropertySetting(edit_impl, sprite)
{
	m_type = "Text";
}

void PropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "LabelSize") {
		double w, h;
		SplitString2Double(value, &w, &h);
		spr->m_width = w;
		spr->m_height = h;
		spr->BuildBounding();
	} else if (name == "LabelSize.Width") {
		spr->m_width = wxANY_AS(value, float);
		spr->BuildBounding();
	} else if (name == "LabelSize.Height") {
		spr->m_height = wxANY_AS(value, float);
		spr->BuildBounding();
	} else if (name == "Font") {
		spr->m_font = wxANY_AS(value, wxString);
	} else if (name == "FontSize") {
		spr->m_font_size = wxANY_AS(value, float);
	} else if (name == "Edge") {
		spr->m_has_edge = wxANY_AS(value, bool);
	} else if (name == "FontColor") {
		wxColour col = wxANY_AS(value, wxColour);
		spr->m_color.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
	} else if (name == "Align.Hori") {
		spr->m_align_hori = HoriAlignType(wxANY_AS(value, int));
	} else if (name == "Align.Vert") {
		spr->m_align_vert = VertAlignType(wxANY_AS(value, int));
	} else if (name == "TextContent") {
		spr->m_text = wxANY_AS(value, wxString);
		m_stage->SetCanvasDirty();
	} else if (name == "TextID") {
		spr->m_tid = wxANY_AS(value, wxString);
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->GetProperty("LabelSize.Width")->SetValue(spr->m_width);
	pg->GetProperty("LabelSize.Height")->SetValue(spr->m_height);

	pg->GetProperty("Font")->SetValue(spr->m_font);
	pg->GetProperty("FontSize")->SetValue(spr->m_font_size);

	pg->GetProperty("Edge")->SetValue(spr->m_has_edge);

	wxColour col = wxColour(spr->m_color.r*255, spr->m_color.g*255, spr->m_color.b*255, spr->m_color.a*255);
	pg->SetPropertyValueString("FontColor", col.GetAsString());

	pg->GetProperty("Align.Hori")->SetValue(HORI_ALIGN_LABELS[spr->m_align_hori]);
	pg->GetProperty("Align.Vert")->SetValue(VERT_ALIGN_LABELS[spr->m_align_vert]);

	pg->GetProperty("TextContent")->SetValue(spr->m_text);
	pg->GetProperty("TextID")->SetValue(spr->m_tid);
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("FONT", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	wxPGProperty* sz_prop = pg->Append(new wxStringProperty("LabelSize", wxPG_LABEL, "<composed>"));
	sz_prop->SetExpanded(false);
	pg->AppendIn(sz_prop, new wxFloatProperty("Width", wxPG_LABEL, spr->m_width));
	pg->AppendIn(sz_prop, new wxFloatProperty("Height", wxPG_LABEL, spr->m_height));

	pg->Append(new wxStringProperty("Font", wxPG_LABEL, spr->m_font));
	pg->Append(new wxFloatProperty("FontSize", wxPG_LABEL, spr->m_font_size));

	pg->Append(new wxBoolProperty("Edge", wxPG_LABEL, spr->m_has_edge));
	pg->SetPropertyAttribute("Edge", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	wxColour col = wxColour(spr->m_color.r*255, spr->m_color.g*255, spr->m_color.b*255, spr->m_color.a*255);
	pg->Append(new wxColourProperty("FontColor", wxPG_LABEL, col));
	pg->SetPropertyAttribute("FontColor", "HasAlpha", true);

	wxPGProperty* align_prop = pg->Append(new wxStringProperty("Align", wxPG_LABEL, "<composed>"));
	align_prop->SetExpanded(false);

	wxEnumProperty* hori_align_prop = new wxEnumProperty("Hori", wxPG_LABEL, HORI_ALIGN_LABELS);
	hori_align_prop->SetValue(HORI_ALIGN_LABELS[spr->m_align_hori]);
	pg->AppendIn(align_prop, hori_align_prop);

	wxEnumProperty* vert_align_prop = new wxEnumProperty("Vert", wxPG_LABEL, VERT_ALIGN_LABELS);
	vert_align_prop->SetValue(VERT_ALIGN_LABELS[spr->m_align_vert]);
	pg->AppendIn(align_prop, vert_align_prop);

	pg->Append(new wxStringProperty("TextContent", wxPG_LABEL, spr->m_text));
	pg->Append(new wxStringProperty("TextID", wxPG_LABEL, spr->m_tid));
}

}