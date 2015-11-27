#include "SpritePropertySetting.h"
#include "SpritePropertyImpl.h"
#include "PropertySettingPanel.h"
#include "PropertyColorMonitor.h"

#include "common/Math.h"
#include "common/color_trans.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "view/IStageCanvas.h"
#include "view/EditPanelImpl.h"
#include "render/BlendModes.h"
#include "widgets/ColorProperty.h"
#include "message/subject_id.h"
#include "message/SpriteNameChangeSJ.h"
#include "message/SetCanvasDirtySJ.h"
#include "message/ResetViewportSJ.h"

#include <wx/propgrid/advprops.h>

namespace d2d
{

SpritePropertySetting::SpritePropertySetting(EditPanelImpl* stage, ISprite* sprite)
	: IPropertySetting("Sprite")
	, m_impl(new SpritePropertyImpl(stage, sprite))
	, m_pg(NULL)
{
	SpriteNameChangeSJ::Instance()->Register(this);
}

SpritePropertySetting::~SpritePropertySetting()
{
	SpriteNameChangeSJ::Instance()->UnRegister(this);
	delete m_impl;
}

void SpritePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	ISprite* spr = m_impl->GetSprite();

	bool dirty = true;

	// base
	if (name == wxT("Name"))
	{
		spr->name = wxANY_AS(value, wxString);
		SpriteNameChangeSJ::Instance()->OnSpriteNameChanged(spr, this);
	}
	else if (name == "Tag")
	{
		spr->tag = wxANY_AS(value, wxString);
	}
	// color
	else if (name == "Color.Multi" && Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT)
	{
		wxColour col = wxANY_AS(value, wxColour);
		spr->multiCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, spr->multiCol.a);
	}
	else if (name == "Color.Add" && Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT)
	{
		wxColour col = wxANY_AS(value, wxColour);
		spr->addCol.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, spr->addCol.a);
	}
	else if (name == "Color.Alpha")
	{
		int alpha = wxANY_AS(value, int);
		alpha = std::max(0, std::min(255, alpha));
		spr->multiCol.a = alpha / 255.0f;
	}
// 	else if (name == "Color.R")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->r_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.G")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->g_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
// 	else if (name == "Color.B")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->b_trans.set(col.Red() / 255.0f, col.Green() / 255.0f, col.Blue() / 255.0f, col.Alpha() / 255.0f);
// 	}
	else if (name == "Blend")
	{
		int idx = wxANY_AS(value, int);
		spr->SetBlendMode(BlendModes::Instance()->GetIDFromIdx(idx));
	}
	else if (name == "Filter")
	{
		int idx = wxANY_AS(value, int);
		spr->SetFilterMode(FilterModes::Instance()->GetIDFromIdx(idx));
		ResetViewportSJ::Instance()->Reset();
	}
	else if (name == wxT("Clip"))
	{
		spr->clip = wxANY_AS(value, bool);
	}
	else if (name == "Anchor")
	{
		spr->SetAnchor(wxANY_AS(value, bool));
	}
	// pos
	else if (name == wxT("Pos"))
	{
 		double x, y;
 		SplitString2Double(value, &x, &y);
 		m_impl->Translate(x, y);
	}
	// angle
	else if (name == wxT("Angle"))
	{
		m_impl->Rotate(wxANY_AS(value, float) * TRANS_DEG_TO_RAD);
	}
	// scale
	else if (name == wxT("Scale"))
	{
		double x, y;
		SplitString2Double(value, &x, &y);
		m_impl->Scale(x, y);
	}
	// size
	else if (name == wxT("Size"))
	{
		double w, h;
		SplitString2Double(value, &w, &h);
		m_impl->Scale(w/spr->GetSymbol().GetSize().xLength(), h/spr->GetSymbol().GetSize().yLength());
	}
	// shear
	else if (name == wxT("Shear"))
	{
		double x, y;
		SplitString2Double(value, &x, &y);
		m_impl->Shear(x, y);
	}
	// offset
	else if (name == wxT("Offset"))
	{
		double x, y;
		SplitString2Double(value, &x, &y);
		m_impl->Offset(x, y);
	}
	// mirror
	else if (name == wxT("Mirror"))
	{
		// todo
	}
	else if (name == wxT("Mirror.Horizontal"))
	{
		bool xMirror, yMirror;
		spr->GetMirror(xMirror, yMirror);
//		spr->setMirror(wxANY_AS(value, bool), yMirror);
		m_impl->Mirror(wxANY_AS(value, bool), yMirror);
	}
	else if (name == wxT("Mirror.Vertical"))
	{
		bool xMirror, yMirror;
		spr->GetMirror(xMirror, yMirror);
//		spr->setMirror(xMirror, wxANY_AS(value, bool));
		m_impl->Mirror(xMirror, wxANY_AS(value, bool));
	}
	// perspective
	else if (name == wxT("Perspective"))
	{
		double x, y;
		SplitString2Double(value, &x, &y);
		m_impl->Perspective(x, y);
	}
	// other
	else if (name == wxT("Visiable"))
	{
		spr->visiable = wxANY_AS(value, bool);
	}
	else if (name == wxT("Editable"))
	{
		spr->editable = wxANY_AS(value, bool);
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SpritePropertySetting::Notify(int sj_id, void* ud)
{
	if (sj_id == MSG_SPRITE_NAME_CHANGE) {
		ISprite* spr = (ISprite*)ud;
		if (GetSprite() == spr && m_pg) {
			m_pg->GetProperty(wxT("Name"))->SetValue(spr->name);	
		}
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	ISprite* spr = m_impl->GetSprite();

	pg->GetProperty(wxT("Name"))->SetValue(spr->name);
	pg->GetProperty(wxT("Tag"))->SetValue(spr->tag);

	if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT) {
		wxColour mul_col = wxColour(spr->multiCol.r*255, spr->multiCol.g*255, spr->multiCol.b*255, spr->multiCol.a*255);
		wxColour add_col = wxColour(spr->addCol.r*255, spr->addCol.g*255, spr->addCol.b*255, spr->addCol.a*255);
		pg->SetPropertyValueString(wxT("Color.Multi"), mul_col.GetAsString());
		pg->SetPropertyValueString(wxT("Color.Add"), add_col.GetAsString());
	}
	pg->GetProperty(wxT("Color.Alpha"))->SetValue((int)(spr->multiCol.a*255));

// 	wxColour r_trans = wxColour(spr->r_trans.r*255, spr->r_trans.g*255, spr->r_trans.b*255, spr->r_trans.a*255);
// 	wxColour g_trans = wxColour(spr->g_trans.r*255, spr->g_trans.g*255, spr->g_trans.b*255, spr->g_trans.a*255);
// 	wxColour b_trans = wxColour(spr->b_trans.r*255, spr->b_trans.g*255, spr->b_trans.b*255, spr->b_trans.a*255);
// 	pg->SetPropertyValueString(wxT("Color.R"), r_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.G"), g_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.B"), b_trans.GetAsString());

	BlendMode blend = spr->GetBlendMode();
	pg->GetProperty(wxT("Blend"))->SetValue(BlendModes::Instance()->GetIdxFromID(blend));

	FilterMode filter = spr->GetFilterMode();
	pg->GetProperty(wxT("Filter"))->SetValue(FilterModes::Instance()->GetIdxFromID(filter));

	ColorProperty* rp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.R"));
	rp->SetListener(new PropertyColorListener(&spr->r_trans));

	ColorProperty* gp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.G"));
	gp->SetListener(new PropertyColorListener(&spr->g_trans));

	ColorProperty* bp = static_cast<ColorProperty*>(pg->GetProperty("Color Conversion.B"));
	bp->SetListener(new PropertyColorListener(&spr->b_trans));

	pg->GetProperty(wxT("Clip"))->SetValue(spr->clip);

	pg->GetProperty(wxT("Anchor"))->SetValue(spr->IsAnchor());

	pg->GetProperty(wxT("Pos.X"))->SetValue(spr->GetPosition().x);
	pg->GetProperty(wxT("Pos.Y"))->SetValue(spr->GetPosition().y);
	pg->GetProperty(wxT("Pos"))->SetValue(pg->GetProperty(wxT("Pos"))->GenerateComposedValue());
	pg->GetProperty(wxT("Angle"))->SetValue(spr->GetAngle() * TRANS_RAD_TO_DEG);
	pg->GetProperty(wxT("Scale.X"))->SetValue(spr->GetScale().x);
	pg->GetProperty(wxT("Scale.Y"))->SetValue(spr->GetScale().y);
	pg->GetProperty(wxT("Scale"))->SetValue(pg->GetProperty(wxT("Scale"))->GenerateComposedValue());
	pg->GetProperty(wxT("Size.Width"))->SetValue(spr->GetSymbol().GetSize(spr).xLength() * spr->GetScale().x);
	pg->GetProperty(wxT("Size.Height"))->SetValue(spr->GetSymbol().GetSize(spr).yLength() * spr->GetScale().y);
	pg->GetProperty(wxT("Size"))->SetValue(pg->GetProperty(wxT("Size"))->GenerateComposedValue());
	pg->GetProperty(wxT("Shear.X"))->SetValue(spr->GetShear().x);
	pg->GetProperty(wxT("Shear.Y"))->SetValue(spr->GetShear().y);
	pg->GetProperty(wxT("Shear"))->SetValue(pg->GetProperty(wxT("Shear"))->GenerateComposedValue());
 	pg->GetProperty(wxT("Offset.X"))->SetValue(spr->GetOffset().x);
	pg->GetProperty(wxT("Offset.Y"))->SetValue(spr->GetOffset().y);
	pg->GetProperty(wxT("Offset"))->SetValue(pg->GetProperty(wxT("Offset"))->GenerateComposedValue());

	bool xMirror, yMirror;
	spr->GetMirror(xMirror, yMirror);
	pg->GetProperty(wxT("Mirror.Horizontal"))->SetValue(xMirror);
	pg->GetProperty(wxT("Mirror.Vertical"))->SetValue(yMirror);
	pg->GetProperty(wxT("Mirror"))->SetValue(pg->GetProperty(wxT("Mirror"))->GenerateComposedValue());

	pg->GetProperty(wxT("Perspective.X"))->SetValue(spr->GetPerspective().x);
	pg->GetProperty(wxT("Perspective.Y"))->SetValue(spr->GetPerspective().y);
	pg->GetProperty(wxT("Perspective"))->SetValue(pg->GetProperty(wxT("Perspective"))->GenerateComposedValue());

	pg->GetProperty(wxT("Visiable"))->SetValue(spr->visiable);
	pg->GetProperty(wxT("Editable"))->SetValue(spr->editable);
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ISprite* spr = m_impl->GetSprite();

	pg->Clear();

	pg->Append(new wxPropertyCategory("BASE", wxPG_LABEL));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, spr->name));

	pg->Append(new wxStringProperty("Tag", wxPG_LABEL, spr->tag));

	pg->Append(new wxBoolProperty("Clip", wxPG_LABEL, spr->clip));
	pg->SetPropertyAttribute("Clip", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty("Anchor", wxPG_LABEL, spr->IsAnchor()));
	pg->SetPropertyAttribute("Anchor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxPropertyCategory("COLOR", wxPG_LABEL));

	wxPGProperty* col_prop = pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, wxT("<composed>")));
	col_prop->SetExpanded(false);

	if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT) {
		wxColour mul_col = wxColour(spr->multiCol.r*255, spr->multiCol.g*255, spr->multiCol.b*255, spr->multiCol.a*255);
		wxColour add_col = wxColour(spr->addCol.r*255, spr->addCol.g*255, spr->addCol.b*255, spr->addCol.a*255);
		pg->AppendIn(col_prop, new wxColourProperty(wxT("Multi"), wxPG_LABEL, mul_col));
		pg->AppendIn(col_prop, new wxColourProperty(wxT("Add"), wxPG_LABEL, add_col));
	} else {
		ColorProperty* multi_prop = new ColorProperty("Multi");
		multi_prop->SetListener(new PropertyColorListener(&spr->multiCol));
		pg->AppendIn(col_prop, multi_prop);

		ColorProperty* add_prop = new ColorProperty("Add");
		add_prop->SetListener(new PropertyColorListener(&spr->addCol));
		pg->AppendIn(col_prop, add_prop);
	}

	pg->AppendIn(col_prop, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, spr->multiCol.a*255));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

	ColorProperty* col_r_prop = new ColorProperty("R");
	col_r_prop->SetListener(new PropertyColorListener(&spr->r_trans));
	pg->AppendIn(col_conv_prop, col_r_prop);

	ColorProperty* col_g_prop = new ColorProperty("G");
	col_g_prop->SetListener(new PropertyColorListener(&spr->g_trans));
	pg->AppendIn(col_conv_prop, col_g_prop);

	ColorProperty* col_b_prop = new ColorProperty("B");
	col_b_prop->SetListener(new PropertyColorListener(&spr->b_trans));
	pg->AppendIn(col_conv_prop, col_b_prop);

// 	wxColour r_trans = wxColour(spr->r_trans.r*255, spr->r_trans.g*255, spr->r_trans.b*255, spr->r_trans.a*255);
// 	wxColour g_trans = wxColour(spr->g_trans.r*255, spr->g_trans.g*255, spr->g_trans.b*255, spr->g_trans.a*255);
// 	wxColour b_trans = wxColour(spr->b_trans.r*255, spr->b_trans.g*255, spr->b_trans.b*255, spr->b_trans.a*255);
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("R"), wxPG_LABEL, r_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("G"), wxPG_LABEL, g_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("B"), wxPG_LABEL, b_trans));

	wxArrayString names;
	BlendModes::Instance()->GetAllNameCN(names);
	wxEnumProperty* blend_prop = new wxEnumProperty(wxT("Blend"), wxPG_LABEL, names);
	int idx = BlendModes::Instance()->GetIdxFromID(spr->GetBlendMode());
	blend_prop->SetValue(idx);
	pg->Append(blend_prop);

	FilterModes::Instance()->GetAllNameCN(names);
	wxEnumProperty* filter_prop = new wxEnumProperty(wxT("Filter"), wxPG_LABEL, names);
	idx = FilterModes::Instance()->GetIdxFromID(spr->GetFilterMode());
	filter_prop->SetValue(idx);
	pg->Append(filter_prop);

	pg->Append(new wxPropertyCategory("GEOMETRY", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetPosition().x));
	pg->SetPropertyAttribute(wxT("Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.X"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetPosition().y));
	pg->SetPropertyAttribute(wxT("Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, spr->GetAngle() * TRANS_RAD_TO_DEG));
	pg->SetPropertyAttribute(wxT("Angle"), wxPG_ATTR_UNITS, wxT("deg"));
	pg->SetPropertyAttribute(wxT("Angle"), "Precision", 1);

	wxPGProperty* scaleProp = pg->Append(new wxStringProperty(wxT("Scale"), wxPG_LABEL, wxT("<composed>")));
	scaleProp->SetExpanded(false);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetScale().x));
	pg->SetPropertyAttribute(wxT("Scale.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.X"), "Precision", 2);
	pg->AppendIn(scaleProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetScale().y));
	pg->SetPropertyAttribute(wxT("Scale.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Scale.Y"), "Precision", 2);

	wxPGProperty* sizeProp = pg->Append(new wxStringProperty(wxT("Size"), wxPG_LABEL, wxT("<composed>")));
	sizeProp->SetExpanded(false);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Width"), wxPG_LABEL, spr->GetSymbol().GetSize().xLength() * spr->GetScale().x));
	pg->SetPropertyAttribute(wxT("Size.Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Width"), "Precision", 2);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Height"), wxPG_LABEL, spr->GetSymbol().GetSize().yLength() * spr->GetScale().y));
	pg->SetPropertyAttribute(wxT("Size.Height"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Height"), "Precision", 2);

	wxPGProperty* shearProp = pg->Append(new wxStringProperty(wxT("Shear"), wxPG_LABEL, wxT("<composed>")));
	shearProp->SetExpanded(false);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetShear().x));
	pg->SetPropertyAttribute(wxT("Shear.X"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.X"), "Precision", 2);
	pg->AppendIn(shearProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetShear().y));
	pg->SetPropertyAttribute(wxT("Shear.Y"), wxPG_ATTR_UNITS, wxT("multiple"));
	pg->SetPropertyAttribute(wxT("Shear.Y"), "Precision", 2);

	wxPGProperty* offsetProp = pg->Append(new wxStringProperty(wxT("Offset"), wxPG_LABEL, wxT("<composed>")));
	offsetProp->SetExpanded(false);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetOffset().x));
	pg->SetPropertyAttribute(wxT("Offset.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.X"), "Precision", 1);
	pg->AppendIn(offsetProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetOffset().y));
	pg->SetPropertyAttribute(wxT("Offset.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Offset.Y"), "Precision", 1);

	wxPGProperty* mirrorProp = pg->Append(new wxStringProperty(wxT("Mirror"), wxPG_LABEL, wxT("<composed>")));
	mirrorProp->SetExpanded(false);
	bool xMirror, yMirror;
	spr->GetMirror(xMirror, yMirror);
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Horizontal"), wxPG_LABEL, xMirror));
	pg->SetPropertyAttribute("Mirror.Horizontal", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Vertical"), wxPG_LABEL, yMirror));
	pg->SetPropertyAttribute("Mirror.Vertical", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	wxPGProperty* persp_prop = pg->Append(new wxStringProperty(wxT("Perspective"), wxPG_LABEL, wxT("<composed>")));
	persp_prop->SetExpanded(false);
	pg->AppendIn(persp_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetPerspective().x));
	pg->SetPropertyAttribute(wxT("Perspective.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Perspective.X"), "Precision", 1);
	pg->AppendIn(persp_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetPerspective().y));
	pg->SetPropertyAttribute(wxT("Perspective.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Perspective.Y"), "Precision", 1);

	pg->Append(new wxPropertyCategory("EDIT", wxPG_LABEL));
	pg->Append(new wxBoolProperty("Visiable", wxPG_LABEL, spr->visiable));
	pg->SetPropertyAttribute("Visiable", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Editable", wxPG_LABEL, spr->editable));
	pg->SetPropertyAttribute("Editable", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

ISprite* SpritePropertySetting::GetSprite()
{
	if (m_impl) {
		return m_impl->GetSprite();
	} else {
		return NULL;
	}
}

} // d2d