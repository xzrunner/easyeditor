#include "MultiSpritesPropertySetting.h"
#include "PropertySettingPanel.h"
#include "MultiSpritesPropertyImpl.h"

#include "common/Math.h"
#include "view/EditPanel.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

MultiSpritesPropertySetting::MultiSpritesPropertySetting(EditPanel* editPanel, const std::vector<ISprite*>& sprites)
	: IPropertySetting(editPanel, wxT("MultiSprites"))
	, m_impl(new MultiSpritesPropertyImpl(sprites))
{
}

MultiSpritesPropertySetting::~MultiSpritesPropertySetting()
{
	delete m_impl;
}

void MultiSpritesPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
		updateProperties(pg);
	else
		initProperties(pg);
}

void MultiSpritesPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	// info
	if (name == wxT("Tag"))
	{
		m_impl->SetTag(wxANY_AS(value, wxString));
	}
	else if (name == wxT("Clip"))
	{
		m_impl->SetClip(wxANY_AS(value, int));
	}
	else if (name == "Color.Multi")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_impl->SetColorMul(Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f));
	}
	else if (name == "Color.Add")
	{
		wxColour col = wxANY_AS(value, wxColour);
		m_impl->SetColorAdd(Colorf(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f));
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
	else if (name == wxT("Align"))
	{
		m_impl->OnAlign(wxANY_AS(value, int));
	}
	else if (name == wxT("Center"))
	{
		m_impl->OnCenter(wxANY_AS(value, int));
	}
	else if (name == wxT("All Pos.X"))
	{
		m_impl->SetPosX(wxANY_AS(value, float));
	}
	else if (name == wxT("All Pos.Y"))
	{
		m_impl->SetPosY(wxANY_AS(value, float));
	}
	// change
	else if (name == wxT("Pos"))
	{
		double dx, dy;
		splitString(value, &dx, &dy);
		m_impl->OnPosChange(dx, dy);
	}
	else if (name == wxT("Angle"))
	{
		float angle = wxANY_AS(value, float) * TRANS_DEG_TO_RAD;
		m_impl->OnAngleChange(angle);
	}
	else if (name == wxT("Scale"))
	{
		double dx, dy;
		splitString(value, &dx, &dy);
		m_impl->OnScaleChange(dx, dy);
	}

	m_editPanel->Refresh();
}

void MultiSpritesPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void MultiSpritesPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
		initProperties(pg);

	pg->GetProperty(wxT("Tag"))->Enable(bEnable);
	pg->GetProperty(wxT("Clip"))->Enable(bEnable);
	pg->GetProperty(wxT("Color"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Multi"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Add"))->Enable(bEnable);
	pg->GetProperty(wxT("Color.Alpha"))->Enable(bEnable);

	pg->GetProperty(wxT("Align"))->Enable(bEnable);
	pg->GetProperty(wxT("Center"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos.X"))->Enable(bEnable);
	pg->GetProperty(wxT("All Pos.Y"))->Enable(bEnable);

	pg->GetProperty(wxT("Pos"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.dx"))->Enable(bEnable);
	pg->GetProperty(wxT("Pos.dy"))->Enable(bEnable);
	pg->GetProperty(wxT("Angle"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.dx"))->Enable(bEnable);
	pg->GetProperty(wxT("Scale.dy"))->Enable(bEnable);
}

void MultiSpritesPropertySetting::updateProperties(wxPropertyGrid* pg)
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

	pg->GetProperty(wxT("Align"))->SetValue(wxT("none"));
	pg->GetProperty(wxT("Center"))->SetValue(wxT("none"));
	Vector pos = m_impl->GetPosition();
	pg->GetProperty(wxT("All Pos.X"))->SetValue(pos.x);
	pg->GetProperty(wxT("All Pos.Y"))->SetValue(pos.y);

	pg->GetProperty(wxT("Pos.dx"))->SetValue(0);
	pg->GetProperty(wxT("Pos.dy"))->SetValue(0);
	pg->GetProperty(wxT("Angle"))->SetValue(0);
	pg->GetProperty(wxT("Scale.dx"))->SetValue(1);
	pg->GetProperty(wxT("Scale.dy"))->SetValue(1);
}

void MultiSpritesPropertySetting::initProperties(wxPropertyGrid* pg)
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
	pg->SetPropertyAttribute(wxT("Color.Multi"), "HasAlpha", true);
	pg->AppendIn(col_prop, new wxColourProperty(wxT("Add"), wxPG_LABEL, wx_add_col));
	pg->SetPropertyAttribute(wxT("Color.Add"), "HasAlpha", true);
	pg->AppendIn(col_prop, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, add_col.a * 255));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

// 	ColorProperty* col_r_prop = new ColorProperty("R");
// 	col_r_prop->SetColor(m_impl->GetTransColorR());
// 	pg->AppendIn(col_conv_prop, col_r_prop);
// 
// 	ColorProperty* col_g_prop = new ColorProperty("G");
// 	col_g_prop->SetColor(m_impl->GetTransColorG());
// 	pg->AppendIn(col_conv_prop, col_g_prop);
// 
// 	ColorProperty* col_b_prop = new ColorProperty("B");
// 	col_b_prop->SetColor(m_impl->GetTransColorB());
// 	pg->AppendIn(col_conv_prop, col_b_prop);

	// position

	pg->Append(new wxPropertyCategory("POSITION", wxPG_LABEL));

	pg->Append(new wxEnumProperty(wxT("Align"), wxPG_LABEL, MultiSpritesPropertyImpl::ALIGN_LABELS));

	pg->Append(new wxEnumProperty(wxT("Center"), wxPG_LABEL, MultiSpritesPropertyImpl::CENTER_LABELS));

	Vector pos = m_impl->GetPosition();
	wxPGProperty* allPosProp = pg->Append(new wxStringProperty(wxT("All Pos"), wxPG_LABEL, wxT("<composed>")));
	allPosProp->SetExpanded(false);
	pg->AppendIn(allPosProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, pos.x));
	pg->SetPropertyAttribute(wxT("All Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.X"), "Precision", 1);
	pg->AppendIn(allPosProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, pos.y));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("All Pos.Y"), "Precision", 1);

	// change

	pg->Append(new wxPropertyCategory("CHANGE", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Pos.dx"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.dx"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Pos.dy"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.dy"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, 0));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scaleProp = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scaleProp->SetExpanded(false);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("dx"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("Scale.dx"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.dx"), "Precision", 2);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("dy"), wxPG_LABEL, 1));
	pg->SetPropertyAttribute(wxT("Scale.dy"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.dy"), "Precision", 2);
}

} // d2d