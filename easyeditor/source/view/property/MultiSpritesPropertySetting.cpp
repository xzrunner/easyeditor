#include "MultiSpritesPropertySetting.h"
#include "PropertySettingPanel.h"
#include "MultiSpritesPropertyImpl.h"
#include "EditPanelImpl.h"
#include "Math2D.h"
#include "StageCanvas.h"
#include "panel_msg.h"

#include <wx/propgrid/advprops.h>

namespace ee
{

MultiSpritesPropertySetting::MultiSpritesPropertySetting(const std::vector<Sprite*>& sprites)
	: PropertySetting("MultiSprites")
	, m_impl(new MultiSpritesPropertyImpl(sprites))
	, m_overall(false)
{
}

MultiSpritesPropertySetting::~MultiSpritesPropertySetting()
{
	delete m_impl;
}

void MultiSpritesPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;

	// info
	if (name == wxT("Tag"))
	{
		m_impl->SetTag(wxANY_AS(value, wxString).ToStdString());
	}
	else if (name == wxT("Clip"))
	{
		m_impl->SetClip(wxANY_AS(value, int));
	}
	else if (name == "Color.Multi")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_impl->SetColorMul(Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
	}
	else if (name == "Color.Add")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_impl->SetColorAdd(Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f));
	}
	else if (name == "Color.Alpha")
	{
		int alpha = wxANY_AS(value, int);
		alpha = std::max(0, std::min(255, alpha));
		m_impl->SetColorAlpha(alpha / 255.0f);
	}
	else if (name == "Color Conversion.R")
	{
		
	}
	// position
	else if (name == wxT("对齐"))
	{
		m_impl->OnAlign(wxANY_AS(value, int));
	}
	else if (name == wxT("居中"))
	{
		m_impl->OnCenter(wxANY_AS(value, int));
	}
	else if (name == wxT("All Pos"))
	{
		double dx, dy;
		SplitString2Double(value, &dx, &dy);
		m_impl->SetPos(dx, dy);
	}
	// change
	else if (name == wxT("dPos"))
	{
		double dx, dy;
		SplitString2Double(value, &dx, &dy);
		m_impl->OnPosChange(dx, dy);
	}
	else if (name == wxT("dAngle"))
	{
		float angle = wxANY_AS(value, float) * SM_DEG_TO_RAD;
		m_impl->OnAngleChange(angle);
	}
	else if (name == wxT("dScale"))
	{
		double dx, dy;
		SplitString2Double(value, &dx, &dy);
		m_impl->OnScaleChange(dx, dy);
	}
	// geometry
	else if (name == wxT("Overall"))
	{
		m_overall = wxANY_AS(value, bool);
	}
	else if (name == "Angle")
	{
		float angle = wxANY_AS(value, float) * SM_DEG_TO_RAD;
		m_impl->SetAngle(m_overall, angle);
	}
	else if (name == "Scale")
	{
		double dx, dy;
		SplitString2Double(value, &dx, &dy);
		m_impl->SetScale(m_overall, dx, dy);
	}
	else if (name == "Mirror.Horizontal")
	{
		m_impl->SetMirrorX(m_overall, wxANY_AS(value, bool));
	}
	else if (name == "Mirror.Vertical")
	{
		m_impl->SetMirrorY(m_overall, wxANY_AS(value, bool));
	}
	// edit
	else if (name == "Visible")
	{
		m_impl->SetVisible(wxANY_AS(value, bool));
	}
	else if (name == "Editable")
	{
		m_impl->SetEditable(wxANY_AS(value, bool));
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void MultiSpritesPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Tag"))->SetValue(m_impl->GetTag());
	pg->GetProperty(wxT("Clip"))->SetValue(m_impl->GetClip());

	Colorf mul_col = m_impl->GetMultiColor();
	Colorf add_col = m_impl->GetAddColor();
	wxColour wx_mul_col = wxColour(mul_col.r*255, mul_col.g*255, mul_col.b*255, mul_col.a*255);
	wxColour wx_add_col = wxColour(add_col.r*255, add_col.g*255, add_col.b*255, add_col.a*255);
	pg->SetPropertyValueString(wxT("Color.Multi"), wx_mul_col.GetAsString());
	pg->SetPropertyValueString(wxT("Color.Add"), wx_add_col.GetAsString());
	pg->GetProperty(wxT("Color.Alpha"))->SetValue((int)(mul_col.a*255));

	pg->GetProperty(wxT("对齐"))->SetValue(wxT("无"));
	pg->GetProperty(wxT("居中"))->SetValue(wxT("无"));
	Vector pos = m_impl->GetPosition();
	pg->GetProperty(wxT("All Pos.X"))->SetValue(pos.x);
	pg->GetProperty(wxT("All Pos.Y"))->SetValue(pos.y);
	pg->GetProperty(wxT("All Pos"))->SetValue(pg->GetProperty(wxT("All Pos"))->GenerateComposedValue());

	pg->GetProperty(wxT("dPos.dx"))->SetValue(0);
	pg->GetProperty(wxT("dPos.dy"))->SetValue(0);
	pg->GetProperty(wxT("dPos"))->SetValue(pg->GetProperty(wxT("dPos"))->GenerateComposedValue());
	pg->GetProperty(wxT("dAngle"))->SetValue(0);
	pg->GetProperty(wxT("dScale.dx"))->SetValue(1);
	pg->GetProperty(wxT("dScale.dy"))->SetValue(1);
	pg->GetProperty(wxT("dScale"))->SetValue(pg->GetProperty(wxT("dScale"))->GenerateComposedValue());

	pg->GetProperty(wxT("Angle"))->SetValue(m_impl->GetAngle());
	Vector scale = m_impl->GetScale();
	pg->GetProperty(wxT("Scale.X"))->SetValue(scale.x);
	pg->GetProperty(wxT("Scale.Y"))->SetValue(scale.y);
	pg->GetProperty(wxT("Scale"))->SetValue(pg->GetProperty(wxT("Scale"))->GenerateComposedValue());
	pg->GetProperty(wxT("Mirror.Horizontal"))->SetValue(m_impl->GetMirrorX());
	pg->GetProperty(wxT("Mirror.Vertical"))->SetValue(m_impl->GetMirrorY());
	pg->GetProperty(wxT("Mirror"))->SetValue(pg->GetProperty(wxT("Mirror"))->GenerateComposedValue());

	pg->GetProperty(wxT("Visible"))->SetValue(m_impl->GetVisible());
	pg->GetProperty(wxT("Editable"))->SetValue(m_impl->GetEditable());
}

void MultiSpritesPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	// info

	pg->Append(new wxPropertyCategory("INFO", wxPG_LABEL));

	pg->Append(new wxStringProperty("Tag", wxPG_LABEL, m_impl->GetTag()));

	pg->Append(new wxEnumProperty(wxT("Clip"), wxPG_LABEL, MultiSpritesPropertyImpl::BOOL_3TYPE_LABELS));
	pg->GetProperty(wxT("Clip"))->SetValue(m_impl->GetClip());

	wxPGProperty* col_prop = pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, wxT("<composed>")));
	Colorf mul_col = m_impl->GetMultiColor();
	Colorf add_col = m_impl->GetAddColor();
	wxColour wx_mul_col = wxColour(mul_col.r*255, mul_col.g*255, mul_col.b*255, mul_col.a*255);
	wxColour wx_add_col = wxColour(add_col.r*255, add_col.g*255, add_col.b*255, add_col.a*255);
	col_prop->SetExpanded(false);
	pg->AppendIn(col_prop, new wxColourProperty(wxT("Multi"), wxPG_LABEL, wx_mul_col));
	pg->AppendIn(col_prop, new wxColourProperty(wxT("Add"), wxPG_LABEL, wx_add_col));
	pg->AppendIn(col_prop, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, add_col.a * 255));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

// 	MyColorProperty* col_r_prop = new MyColorProperty("R");
// 	col_r_prop->SetColor(m_impl->GetTransColorR());
// 	pg->AppendIn(col_conv_prop, col_r_prop);
// 
// 	MyColorProperty* col_g_prop = new MyColorProperty("G");
// 	col_g_prop->SetColor(m_impl->GetTransColorG());
// 	pg->AppendIn(col_conv_prop, col_g_prop);
// 
// 	MyColorProperty* col_b_prop = new MyColorProperty("B");
// 	col_b_prop->SetColor(m_impl->GetTransColorB());
// 	pg->AppendIn(col_conv_prop, col_b_prop);

	// position

	pg->Append(new wxPropertyCategory("POSITION", wxPG_LABEL));

	pg->Append(new wxEnumProperty(wxT("对齐"), wxPG_LABEL, MultiSpritesPropertyImpl::ALIGN_LABELS));

	pg->Append(new wxEnumProperty(wxT("居中"), wxPG_LABEL, MultiSpritesPropertyImpl::CENTER_LABELS));

	Vector pos = m_impl->GetPosition();
	wxPGProperty* all_pos_prop = pg->Append(new wxStringProperty(wxT("All Pos"), wxPG_LABEL, wxT("<composed>")));
	all_pos_prop->SetExpanded(false);
	pg->AppendIn(all_pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, pos.x));
	pg->SetPropertyAttribute(wxT("All Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.X"), "Precision", 1);
	pg->AppendIn(all_pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, pos.y));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), "Precision", 1);

	// change

	pg->Append(new wxPropertyCategory("CHANGE", wxPG_LABEL));

	wxPGProperty* dpos_prop = pg->Append(new wxStringProperty(wxT("dPos"), wxPG_LABEL, wxT("<composed>")));
	dpos_prop->SetExpanded(false);
	pg->AppendIn(dpos_prop, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("dPos.dx"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("dPos.dx"), "Precision", 1);
	pg->AppendIn(dpos_prop, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("dPos.dy"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("dPos.dy"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("dAngle"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("dAngle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("dAngle"), "Precision", 1);

	wxPGProperty* dscale_prop = pg->Append(new wxStringProperty(wxT("dScale"), wxPG_LABEL, wxT("<composed>")));
	dscale_prop->SetExpanded(false);
	pg->AppendIn(dscale_prop, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("dScale.dx"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("dScale.dx"), "Precision", 2);
	pg->AppendIn(dscale_prop, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("dScale.dy"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("dScale.dy"), "Precision", 2);

	// geometry

	pg->Append(new wxPropertyCategory("GEOMETRY", wxPG_LABEL));

	pg->Append(new wxBoolProperty("Overall", wxPG_LABEL, false));
	pg->SetPropertyAttribute("Overall", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->SetPropertyAttribute("Overall", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, m_impl->GetAngle() * SM_RAD_TO_DEG));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scale_prop = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scale_prop->SetExpanded(false);
	pg->AppendIn(scale_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, m_impl->GetScale().x));
	pg->SetPropertyAttribute(wxT("Scale.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.X"), "Precision", 2);
	pg->AppendIn(scale_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_impl->GetScale().y));
	pg->SetPropertyAttribute(wxT("Scale.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.Y"), "Precision", 2);

	wxPGProperty* mirror_prop = pg->Append(new wxStringProperty(wxT("Mirror"), wxPG_LABEL, wxT("<composed>")));
	mirror_prop->SetExpanded(false);
	pg->AppendIn(mirror_prop, new wxBoolProperty(wxT("Horizontal"), wxPG_LABEL, m_impl->GetMirrorX()));
	pg->SetPropertyAttribute("Mirror.Horizontal", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->AppendIn(mirror_prop, new wxBoolProperty(wxT("Vertical"), wxPG_LABEL, m_impl->GetMirrorY()));
	pg->SetPropertyAttribute("Mirror.Vertical", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	// edit

	pg->Append(new wxPropertyCategory("EDIT", wxPG_LABEL));

	pg->Append(new wxBoolProperty("Visible", wxPG_LABEL, m_impl->GetVisible()));
	pg->SetPropertyAttribute("Visible", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Editable", wxPG_LABEL, m_impl->GetEditable()));
	pg->SetPropertyAttribute("Editable", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}