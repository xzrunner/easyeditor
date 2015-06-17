#include "PropertySettingPanel.h"
#include "IPropertySetting.h"

#include "common/visitors.h"
#include "view/MultiSpritesPropertySetting.h"

namespace d2d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_setting(NULL)
	, m_stage(NULL)
{
	InitLayout();

	SetBackgroundColour(wxColour(229, 229, 229));
}

PropertySettingPanel::~PropertySettingPanel()
{
	delete m_setting;
}

void PropertySettingPanel::SelectSprite(ISprite* spr)
{
	assert(m_stage);
	if (spr) {
		SetPropertySetting(spr->CreatePropertySetting(m_stage));
	} else {
		SetPropertySetting(CreateDefaultProperty());
	}
}

void PropertySettingPanel::SelectMultiSprites(SpriteSelection* selection)
{
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	if (sprites.empty()) {
		SelectSprite(NULL);
	} else if (sprites.size() == 1) {
		SelectSprite(sprites[0]);
	} else {
		SetPropertySetting(new MultiSpritesPropertySetting(m_stage, sprites));
	}
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