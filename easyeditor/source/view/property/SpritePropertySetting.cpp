#include "SpritePropertySetting.h"
#include "SpritePropertyImpl.h"
#include "PropertySettingPanel.h"
#include "SprPropColMonitor.h"
#include "Math2D.h"
#include "Config.h"
#include "SettingData.h"
#include "Sprite.h"
#include "Symbol.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "BlendModes.h"
#include "FastBlendModes.h"
#include "MyColorProperty.h"
#include "subject_id.h"
#include "sprite_msg.h"
#include "panel_msg.h"
#include "SetSpriteColorAOP.h"
#include "FileHelper.h"
#include "FilterModes.h"
#include "WXHelper.h"
#include "SprFilterProperty.h"
#include "FileDirProperty.h"
#include "FileNameProperty.h"
#include "EditPanel.h"
#include "SprTagProperty.h"
#include "StringHelper.h"
#include "EditSprMsg.h"

#include <sprite2/RenderColor.h>
#include <sprite2/RenderShader.h>

#include <wx/propgrid/advprops.h>

namespace ee
{

SpritePropertySetting::SpritePropertySetting(EditPanelImpl* stage, Sprite* spr)
	: PropertySetting("Sprite")
	, m_impl(new SpritePropertyImpl(stage, spr))
	, m_pg(NULL)
	, m_spr_filter_prop(stage->GetEditPanel())
{
	RegistSubject(SpriteNameChangeSJ::Instance());
}

SpritePropertySetting::~SpritePropertySetting()
{
	delete m_impl;
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	Sprite* spr = m_impl->GetSprite();
	Symbol* sym = dynamic_cast<Symbol*>(spr->GetSymbol());

	bool dirty = true;

	// res
	if (name == "FileDir")
	{
		std::string file = ee::FileHelper::GetFilenameWithExtension(sym->GetFilepath());
		std::string filepath = wxANY_AS(value, wxString).ToStdString() + "\\" + file;
		dynamic_cast<Symbol*>(sym)->SetFilepath(filepath);
	}
	else if (name == "FileName")
	{
		std::string dir = ee::FileHelper::GetFileDir(sym->GetFilepath());
		std::string filepath = dir + "\\" + wxANY_AS(value, wxString).ToStdString();
		sym->SetFilepath(filepath);
	}
	// base
	else if (name == wxT("Name"))
	{
		spr->SetName(wxANY_AS(value, wxString).ToStdString());
		SpriteNameChangeSJ::Instance()->OnSpriteNameChanged(spr, this);
	}
	else if (name == "Tag")
	{
		spr->SetTag(wxANY_AS(value, wxString).ToStdString());
	}
	// color
	else if (name == "Color")
	{
		std::vector<std::string> tags;
		ee::StringHelper::Split(wxANY_AS(value, wxString).ToStdString(), "; ", tags);
		assert(tags.size() == 4);
		SetColMul(spr, tags[0]);
		SetColAdd(spr, tags[1]);
		SetColAlpha(spr, tags[2]);
		SetColOverlap(spr, tags[3]);
	}
	else if (name == "Color.Multi" && Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT)
	{
		wxColour wx_col = wxANY_AS(value, wxColour);
		SetColMul(spr, wx_col.GetAsString().ToStdString());
	}
	else if (name == "Color.Add" && Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT)
	{
		wxColour wx_col = wxANY_AS(value, wxColour);
		SetColAdd(spr, wx_col.GetAsString().ToStdString());
	}
	else if (name == "Color.Alpha")
	{
		SetColAlpha(spr, wxANY_AS(value, wxString).ToStdString());
	}
	else if (name == "Color.Overlay")
	{
		SetColOverlap(spr, wxANY_AS(value, wxString).ToStdString());
	}
// 	else if (name == "Color.R")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->rp->r_trans.set(col.Red(), col.Green(), col.Blue(), col.Alpha());
// 	}
// 	else if (name == "Color.G")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->rp->g_trans.set(col.Red(), col.Green(), col.Blue(), col.Alpha());
// 	}
// 	else if (name == "Color.B")
// 	{
// 		wxColour col = wxANY_AS(value, wxColour);
// 		spr->rp->b_trans.set(col.Red(), col.Green(), col.Blue(), col.Alpha());
// 	}
	else if (name == "Blend")
	{
		s2::RenderShader rs = spr->GetShader();
		int idx = wxANY_AS(value, int);
		rs.SetBlend(BlendModes::Instance()->ID2Mode(idx));
		spr->SetShader(rs);
	}
	else if (name == "FastBlend") 
	{
		s2::RenderShader rs = spr->GetShader();
		int idx = wxANY_AS(value, int);
		rs.SetFastBlend(FastBlendModes::Instance()->ID2Mode(idx));
		spr->SetShader(rs);
	}
	else if (name == "Anchor")
	{
		spr->SetAnchor(wxANY_AS(value, bool));
	}
	else if (name == "Actor")
	{
		spr->SetNeedActor(wxANY_AS(value, bool));
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
		m_impl->Rotate(wxANY_AS(value, float) * SM_DEG_TO_RAD);
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
		sm::vec2 sz = sym->GetBounding(spr).Size();
		m_impl->Scale(w / sz.x, h / sz.y);
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
//		spr->setMirror(wxANY_AS(value, bool), yMirror);
		m_impl->Mirror(wxANY_AS(value, bool), spr->GetScale().y < 0);
	}
	else if (name == wxT("Mirror.Vertical"))
	{
//		spr->setMirror(xMirror, wxANY_AS(value, bool));
		m_impl->Mirror(spr->GetScale().x < 0, wxANY_AS(value, bool));
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
		spr->SetVisible(wxANY_AS(value, bool));
		RefreshPanelSJ::Instance()->Refresh();
	}
	else if (name == wxT("Editable"))
	{
		spr->SetEditable(wxANY_AS(value, bool));
		RefreshPanelSJ::Instance()->Refresh();
	}
	else if (m_spr_filter_prop.FromPS(name, value, spr))
	{
		dirty = true;
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	Sprite* spr = m_impl->GetSprite();
	Symbol* sym = dynamic_cast<Symbol*>(spr->GetSymbol());

	FileDirProperty* dir_prop = dynamic_cast<FileDirProperty*>(pg->GetProperty(wxT("FileDir")));
	dir_prop->SetFilepath(sym->GetFilepath());
	FileNameProperty* name_prop = dynamic_cast<FileNameProperty*>(pg->GetProperty(wxT("FileName")));
	name_prop->SetFilepath(sym->GetFilepath());

	pg->GetProperty(wxT("Name"))->SetValue(spr->GetName());
	pg->GetProperty(wxT("Tag"))->SetValue(spr->GetTag());

	SprTagProperty* tag_prop = dynamic_cast<SprTagProperty*>(pg->GetProperty(wxT("Tag")));
	tag_prop->SetParent(pg);
	tag_prop->SetSprite(spr);

	const s2::Color& mul = spr->GetColor().GetMul();
	const s2::Color& add = spr->GetColor().GetAdd();
	if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT) 
	{
		wxColour mul_col = wxColour(mul.r, mul.g, mul.b, mul.a);
		wxColour add_col = wxColour(add.r, add.g, add.b, add.a);
		pg->SetPropertyValueString(wxT("Color.Multi"), mul_col.GetAsString());
		pg->SetPropertyValueString(wxT("Color.Add"), add_col.GetAsString());
	}
	pg->GetProperty(wxT("Color.Alpha"))->SetValue(mul.a);
	pg->GetProperty(wxT("Color.Overlay"))->SetValue(add.a);

// 	wxColour r_trans = wxColour(spr->rp->r_trans.r, spr->rp->r_trans.g, spr->rp->r_trans.b, spr->rp->r_trans.a);
// 	wxColour g_trans = wxColour(spr->rp->g_trans.r, spr->rp->g_trans.g, spr->rp->g_trans.b, spr->rp->g_trans.a);
// 	wxColour b_trans = wxColour(spr->rp->b_trans.r, spr->rp->b_trans.g, spr->rp->b_trans.b, spr->rp->b_trans.a);
// 	pg->SetPropertyValueString(wxT("Color.R"), r_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.G"), g_trans.GetAsString());
// 	pg->SetPropertyValueString(wxT("Color.B"), b_trans.GetAsString());

	pg->GetProperty(wxT("Blend"))->SetValue(BlendModes::Instance()->Mode2ID(spr->GetShader().GetBlend()));

	pg->GetProperty(wxT("FastBlend"))->SetValue(FastBlendModes::Instance()->Mode2ID(spr->GetShader().GetFastBlend()));

	m_spr_filter_prop.ToPS(spr, pg);

	MyColorProperty* rp = static_cast<MyColorProperty*>(pg->GetProperty("Color Conversion.R"));
	rp->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_RMAP));

	MyColorProperty* gp = static_cast<MyColorProperty*>(pg->GetProperty("Color Conversion.G"));
	gp->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_GMAP));

	MyColorProperty* bp = static_cast<MyColorProperty*>(pg->GetProperty("Color Conversion.B"));
	bp->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_BMAP));

	pg->GetProperty(wxT("Anchor"))->SetValue(spr->IsAnchor());

	pg->GetProperty(wxT("Actor"))->SetValue(spr->IsNeedActor());

	pg->GetProperty(wxT("Pos.X"))->SetValue(spr->GetPosition().x);
	pg->GetProperty(wxT("Pos.Y"))->SetValue(spr->GetPosition().y);
	pg->GetProperty(wxT("Pos"))->SetValue(pg->GetProperty(wxT("Pos"))->GenerateComposedValue());
	pg->GetProperty(wxT("Angle"))->SetValue(spr->GetAngle() * SM_RAD_TO_DEG);
	pg->GetProperty(wxT("Scale.X"))->SetValue(spr->GetScale().x);
	pg->GetProperty(wxT("Scale.Y"))->SetValue(spr->GetScale().y);
	pg->GetProperty(wxT("Scale"))->SetValue(pg->GetProperty(wxT("Scale"))->GenerateComposedValue());
	sm::vec2 sz = spr->GetSymbol()->GetBounding(spr).Size();
	pg->GetProperty(wxT("Size.Width"))->SetValue(sz.x * spr->GetScale().x);
	pg->GetProperty(wxT("Size.Height"))->SetValue(sz.y * spr->GetScale().y);
	pg->GetProperty(wxT("Size"))->SetValue(pg->GetProperty(wxT("Size"))->GenerateComposedValue());
	pg->GetProperty(wxT("Shear.X"))->SetValue(spr->GetShear().x);
	pg->GetProperty(wxT("Shear.Y"))->SetValue(spr->GetShear().y);
	pg->GetProperty(wxT("Shear"))->SetValue(pg->GetProperty(wxT("Shear"))->GenerateComposedValue());
 	pg->GetProperty(wxT("Offset.X"))->SetValue(spr->GetOffset().x);
	pg->GetProperty(wxT("Offset.Y"))->SetValue(spr->GetOffset().y);
	pg->GetProperty(wxT("Offset"))->SetValue(pg->GetProperty(wxT("Offset"))->GenerateComposedValue());

	pg->GetProperty(wxT("Mirror.Horizontal"))->SetValue(spr->GetScale().x < 0);
	pg->GetProperty(wxT("Mirror.Vertical"))->SetValue(spr->GetScale().y < 0);
	pg->GetProperty(wxT("Mirror"))->SetValue(pg->GetProperty(wxT("Mirror"))->GenerateComposedValue());

	pg->GetProperty(wxT("Perspective.X"))->SetValue(spr->GetPerspective().x);
	pg->GetProperty(wxT("Perspective.Y"))->SetValue(spr->GetPerspective().y);
	pg->GetProperty(wxT("Perspective"))->SetValue(pg->GetProperty(wxT("Perspective"))->GenerateComposedValue());

	pg->GetProperty(wxT("Visiable"))->SetValue(spr->IsVisible());
	pg->GetProperty(wxT("Editable"))->SetValue(spr->IsEditable());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	Sprite* spr = m_impl->GetSprite();
	Symbol* sym = dynamic_cast<Symbol*>(spr->GetSymbol());

	pg->Clear();

	pg->Append(new wxPropertyCategory("RES", wxPG_LABEL));

	pg->Append(new FileDirProperty("FileDir", wxPG_LABEL, sym->GetFilepath()));
	pg->Append(new FileNameProperty("FileName", wxPG_LABEL, sym->GetFilepath()));

	pg->Append(new wxPropertyCategory("BASE", wxPG_LABEL));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, spr->GetName()));

	SprTagProperty* tag_prop = new SprTagProperty("Tag", wxPG_LABEL, spr->GetTag());
	tag_prop->SetParent(pg);
	tag_prop->SetSprite(spr);
	pg->Append(tag_prop);

	pg->Append(new wxBoolProperty("Anchor", wxPG_LABEL, spr->IsAnchor()));
	pg->SetPropertyAttribute("Anchor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty("Actor", wxPG_LABEL, spr->IsNeedActor()));
	pg->SetPropertyAttribute("Actor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxPropertyCategory("COLOR", wxPG_LABEL));

	wxPGProperty* col_prop = pg->Append(new wxStringProperty(wxT("Color"), wxPG_LABEL, wxT("<composed>")));
	col_prop->SetExpanded(false);

	const s2::Color& mul = spr->GetColor().GetMul();
	const s2::Color& add = spr->GetColor().GetAdd();
	if (Config::Instance()->GetSettings().color_setting_dlg_type == CSDT_DEFAULT) {
		wxColour mul_col = wxColour(mul.r, mul.g, mul.b, mul.a);
		wxColour add_col = wxColour(add.r, add.g, add.b, add.a);
		pg->AppendIn(col_prop, new wxColourProperty(wxT("Multi"), wxPG_LABEL, mul_col));
		pg->AppendIn(col_prop, new wxColourProperty(wxT("Add"), wxPG_LABEL, add_col));
	} else {
		MyColorProperty* multi_prop = new MyColorProperty("Multi");
		multi_prop->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_MUL));
		pg->AppendIn(col_prop, multi_prop);

		MyColorProperty* add_prop = new MyColorProperty("Add");
		add_prop->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_ADD));
		pg->AppendIn(col_prop, add_prop);
	}

	pg->AppendIn(col_prop, new wxIntProperty(wxT("Alpha"), wxPG_LABEL, mul.a));
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Alpha"), "Max", 255);

	pg->AppendIn(col_prop, new wxIntProperty(wxT("Overlay"), wxPG_LABEL, add.a));
	pg->SetPropertyAttribute(wxT("Color.Overlay"), "Min", 0);
	pg->SetPropertyAttribute(wxT("Color.Overlay"), "Max", 255);

	wxPGProperty* col_conv_prop = pg->Append(new wxStringProperty(wxT("Color Conversion"), wxPG_LABEL, wxT("<composed>")));
	col_conv_prop->SetExpanded(false);

	MyColorProperty* col_r_prop = new MyColorProperty("R");
	col_r_prop->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_RMAP));
	pg->AppendIn(col_conv_prop, col_r_prop);

	MyColorProperty* col_g_prop = new MyColorProperty("G");
	col_g_prop->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_GMAP));
	pg->AppendIn(col_conv_prop, col_g_prop);

	MyColorProperty* col_b_prop = new MyColorProperty("B");
	col_b_prop->SetListener(new SprPropColMonitor(spr, SprPropColMonitor::CT_BMAP));
	pg->AppendIn(col_conv_prop, col_b_prop);

// 	wxColour r_trans = wxColour(spr->rp->r_trans.r, spr->rp->r_trans.g, spr->rp->r_trans.b, spr->rp->r_trans.a);
// 	wxColour g_trans = wxColour(spr->rp->g_trans.r, spr->rp->g_trans.g, spr->rp->g_trans.b, spr->rp->g_trans.a);
// 	wxColour b_trans = wxColour(spr->rp->b_trans.r, spr->rp->b_trans.g, spr->rp->b_trans.b, spr->rp->b_trans.a);
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("R"), wxPG_LABEL, r_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("G"), wxPG_LABEL, g_trans));
// 	pg->AppendIn(colProp, new wxColourProperty(wxT("B"), wxPG_LABEL, b_trans));

	std::vector<std::string> blend_names;
	BlendModes::Instance()->GetAllNameCN(blend_names);
	wxEnumProperty* blend_prop = new wxEnumProperty(wxT("Blend"), wxPG_LABEL, WXHelper::ToWXStringArray(blend_names));
	int blend_idx = BlendModes::Instance()->Mode2ID(spr->GetShader().GetBlend());
	blend_prop->SetValue(blend_idx);
	pg->Append(blend_prop);

	std::vector<std::string> fast_blend_names;
	FastBlendModes::Instance()->GetAllNameCN(fast_blend_names);
	wxEnumProperty* fast_blend_prop = new wxEnumProperty(wxT("FastBlend"), wxPG_LABEL, WXHelper::ToWXStringArray(fast_blend_names));
	int fast_blend_idx = FastBlendModes::Instance()->Mode2ID(spr->GetShader().GetFastBlend());
	fast_blend_prop->SetValue(fast_blend_idx);
	pg->Append(fast_blend_prop);

	m_spr_filter_prop.InitPS(spr, pg);

	pg->Append(new wxPropertyCategory("GEOMETRY", wxPG_LABEL));

	wxPGProperty* posProp = pg->Append(new wxStringProperty(wxT("Pos"), wxPG_LABEL, wxT("<composed>")));
	posProp->SetExpanded(false);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("X"), wxPG_LABEL, spr->GetPosition().x));
	pg->SetPropertyAttribute(wxT("Pos.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.X"), "Precision", 1);
	pg->AppendIn(posProp, new wxFloatProperty(wxT("Y"), wxPG_LABEL, spr->GetPosition().y));
	pg->SetPropertyAttribute(wxT("Pos.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos.Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Angle"), wxPG_LABEL, spr->GetAngle() * SM_RAD_TO_DEG));
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
	sm::vec2 sz = sym->GetBounding().Size();
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Width"), wxPG_LABEL, sz.x * spr->GetScale().x));
	pg->SetPropertyAttribute(wxT("Size.Width"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Size.Width"), "Precision", 2);
	pg->AppendIn(sizeProp, new wxFloatProperty(wxT("Height"), wxPG_LABEL, sz.y * spr->GetScale().y));
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
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Horizontal"), wxPG_LABEL, spr->GetScale().x < 0));
	pg->SetPropertyAttribute("Mirror.Horizontal", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->AppendIn(mirrorProp, new wxBoolProperty(wxT("Vertical"), wxPG_LABEL, spr->GetScale().y < 0));
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
	pg->Append(new wxBoolProperty("Visiable", wxPG_LABEL, spr->IsVisible()));
	pg->SetPropertyAttribute("Visiable", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Editable", wxPG_LABEL, spr->IsEditable()));
	pg->SetPropertyAttribute("Editable", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

void SpritePropertySetting::OnNotify(int sj_id, void* ud)
{
	if (sj_id == MSG_SPRITE_NAME_CHANGE) {
		Sprite* spr = (Sprite*)ud;
		if (GetSprite() == spr && m_pg) {
			m_pg->GetProperty(wxT("Name"))->SetValue(spr->GetName());	
		}
	}
}

Sprite* SpritePropertySetting::GetSprite()
{
	if (m_impl) {
		return m_impl->GetSprite();
	} else {
		return NULL;
	}
}

void SpritePropertySetting::SetColMul(ee::Sprite* spr, const std::string& val)
{
	std::string fixed = val;
	if (!fixed.empty() && val[0] == '(') {
		fixed = "rgb" + fixed;
	}
	wxColour wx_col(fixed);
	const s2::Color& mul = spr->GetColor().GetMul();
	s2::Color col(wx_col.Red(), wx_col.Green(), wx_col.Blue(), mul.a);
	if (col == mul) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new SetSpriteMulColorAOP(spr, col));
	s2::RenderColor rc = spr->GetColor();
	rc.SetMul(col);
	spr->SetColor(rc);

	EditSprMsg::SetColMul(spr, col);
}

void SpritePropertySetting::SetColAdd(ee::Sprite* spr, const std::string& val)
{
	std::string fixed = val;
	if (!fixed.empty() && val[0] == '(') {
		fixed = "rgb" + fixed;
	}
	wxColour wx_col(fixed);
	const s2::Color& add = spr->GetColor().GetAdd();
	s2::Color col(wx_col.Red(), wx_col.Green(), wx_col.Blue(), add.a);
	if (col == add) {
		return;
	}

	EditAddRecordSJ::Instance()->Add(new SetSpriteAddColorAOP(spr, col));
	s2::RenderColor rc = spr->GetColor();
	rc.SetAdd(col);
	spr->SetColor(rc);

	EditSprMsg::SetColAdd(spr, col);
}

void SpritePropertySetting::SetColAlpha(ee::Sprite* spr, const std::string& val)
{
	int alpha;
	ee::StringHelper::FromString(val, alpha);
	alpha = std::max(0, std::min(255, alpha));

	if (spr->GetColor().GetMul().a == alpha) {
		return;
	}

	s2::Color col = spr->GetColor().GetMul();
	col.a = alpha;
	EditAddRecordSJ::Instance()->Add(new SetSpriteMulColorAOP(spr, col));

	s2::RenderColor rc = spr->GetColor();
	rc.SetMul(col);
	spr->SetColor(rc);

	EditSprMsg::SetColMul(spr, col);
}

void SpritePropertySetting::SetColOverlap(ee::Sprite* spr, const std::string& val)
{
	int overlay;
	ee::StringHelper::FromString(val, overlay);
	overlay = std::max(0, std::min(255, overlay));
	if (spr->GetColor().GetAdd().a == overlay) {
		return;
	}

	s2::Color add = spr->GetColor().GetAdd();
	add.a = overlay;
	EditAddRecordSJ::Instance()->Add(new SetSpriteAddColorAOP(spr, add));

	s2::RenderColor rc = spr->GetColor();
	rc.SetAdd(add);
	spr->SetColor(rc);

	EditSprMsg::SetColAdd(spr, add);
}

}