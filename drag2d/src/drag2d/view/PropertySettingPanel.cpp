#include "PropertySettingPanel.h"
#include "IPropertySetting.h"

#include "common/visitors.h"
#include "dataset/ISprite.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/SpriteSelection.h"
#include "view/ShapeSelection.h"
#include "message/subject_id.h"
#include "message/SpriteSelectedSJ.h"
#include "message/MultiSpriteSelectedSJ.h"

namespace d2d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_setting(NULL)
	, m_stage(NULL)
{
	InitLayout();

	SetBackgroundColour(wxColour(229, 229, 229));

	SpriteSelectedSJ::Instance()->Register(this);
	MultiSpriteSelectedSJ::Instance()->Register(this);
}

PropertySettingPanel::~PropertySettingPanel()
{
	SpriteSelectedSJ::Instance()->UnRegister(this);
	MultiSpriteSelectedSJ::Instance()->UnRegister(this);

	delete m_setting;
}

bool PropertySettingPanel::InsertSprite(ISprite* spr, int idx) 
{ 
	SetPropertySetting(CreateDefaultProperty());
	return false; 
}

bool PropertySettingPanel::RemoveSprite(ISprite* spr) 
{ 
	SetPropertySetting(CreateDefaultProperty());
	return false; 
}

bool PropertySettingPanel::ClearAllSprite() 
{ 
	SetPropertySetting(CreateDefaultProperty());
	return false; 
}

void PropertySettingPanel::SelectShape(IShape* shape)
{
	assert(m_stage);
	if (shape) {
		SetPropertySetting(shape->createPropertySetting(m_stage));
	} else {
		SetPropertySetting(CreateDefaultProperty());
	}
}

void PropertySettingPanel::SelectMultiShapes(ShapeSelection* selection)
{
	std::vector<IShape*> shapes;
	selection->Traverse(FetchAllVisitor<IShape>(shapes));
	if (shapes.empty()) {
		SelectShape(NULL);
	} else if (shapes.size() == 1) {
		SelectShape(shapes[0]);
	} else {
		SelectShape(NULL);
	}
}

void PropertySettingPanel::RemoveShape(IShape* shape)
{
	SetPropertySetting(CreateDefaultProperty());
}

void PropertySettingPanel::Notify(int sj_id, void* ud)
{
	if (sj_id == SPRITE_SELECTED) {
		SpriteSelectedSJ::Params* p = (SpriteSelectedSJ::Params*)ud;
		OnSpriteSelected(p->spr, p->clear);
	} else if (sj_id == MULTI_SPRITE_SELECTED) {
		SpriteSelection* selection = (SpriteSelection*)ud;
		OnMultiSpriteSelected(selection);
	}
}

void PropertySettingPanel::SetPropertySetting(IPropertySetting* setting)
{
	delete m_setting;
	m_setting = setting;

	if (m_setting) {
		m_setting->UpdateFromPanel(this);
		m_type = m_setting->GetType();
	} else {
		m_pg->Clear();
		m_type = "";
	}
}

void PropertySettingPanel::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	m_setting->OnPropertyGridChange(property->GetName(), property->GetValue());
}

void PropertySettingPanel::UpdatePropertyGrid()
{
	if (m_setting) {
		m_setting->UpdateFromPanel(this);
	}
}

void PropertySettingPanel::EnablePropertyGrid(bool enable)
{
	if (m_setting) {
		m_setting->EnablePropertyGrid(this, enable);
	}
}

void PropertySettingPanel::OnSpriteSelected(d2d::ISprite* spr, bool clear)
{
	assert(m_stage);
	if (spr) {
		SetPropertySetting(spr->CreatePropertySetting(m_stage));
	} else {
		SetPropertySetting(CreateDefaultProperty());
	}
}

void PropertySettingPanel::OnMultiSpriteSelected(SpriteSelection* selection)
{
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	if (sprites.empty()) {
		OnSpriteSelected(NULL, true);
	} else if (sprites.size() == 1) {
		OnSpriteSelected(sprites[0], true);
	} else {
		SetPropertySetting(new MultiSpritesPropertySetting(m_stage, sprites));
	}
}

void PropertySettingPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Property"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	// pg
	m_pg = new wxPropertyGrid(
		this, 
		-1,
		wxDefaultPosition,
		wxSize(400, 800),
		wxPG_SPLITTER_AUTO_CENTER
		| wxPG_BOLD_MODIFIED
		);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertySettingPanel::OnPropertyGridChange));
	sizer->Add(m_pg);

	SetSizer(sizer);
}

} // d2d