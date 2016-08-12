#include "PropertySettingPanel.h"
#include "PropertySetting.h"
#include "Sprite.h"
#include "MultiSpritesPropertySetting.h"
#include "SpriteSelection.h"
#include "ShapeSelection.h"
#include "panel_msg.h"
#include "sprite_msg.h"
#include "shape_msg.h"
#include "FetchAllVisitor.h"
#include "subject_id.h"

#include <algorithm>

namespace ee
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_setting(NULL)
	, m_stage(NULL)
{
	InitLayout();

	SetBackgroundColour(wxColour(229, 229, 229));

	RegistSubject(SelectSpriteSJ::Instance());
	RegistSubject(SelectSpriteSetSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());

	RegistSubject(SelectShapeSJ::Instance());
	RegistSubject(SelectShapeSetSJ::Instance());
	RegistSubject(RemoveShapeSJ::Instance());

	RegistSubject(ClearPanelSJ::Instance());
}

PropertySettingPanel::~PropertySettingPanel()
{
	ClearSelection();
	delete m_setting;
}

void PropertySettingPanel::SetDefaultPropertySetting()
{
	SetPropertySetting(CreateDefaultProperty());
}

void PropertySettingPanel::SetPropertySetting(PropertySetting* setting)
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
	m_setting->OnPropertyGridChange(property->GetName().ToStdString(), property->GetValue());
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

void PropertySettingPanel::OnSpriteSelected(Sprite* spr, bool clear)
{
	if (spr) {
		std::set<Sprite*>::iterator itr = m_selection.find(spr);
		if (itr != m_selection.end()) {
			if (!clear) {
				return;
			}
		}
	}

	if (clear) {
		ClearSelection();
	}
	if (spr) {
		spr->AddReference();
		m_selection.insert(spr);
	}

	assert(m_stage);
	if (m_selection.size() == 0) {
		SetPropertySetting(CreateDefaultProperty());
	} else if (m_selection.size() == 1) {
		if (spr) {
			SetPropertySetting(spr->CreatePropertySetting(m_stage));
		} else {
			SetPropertySetting(CreateDefaultProperty());
		}
	} else if (m_selection.size() > 1) {
		std::vector<Sprite*> sprites;
		std::copy(m_selection.begin(), m_selection.end(), back_inserter(sprites));
		SetPropertySetting(new MultiSpritesPropertySetting(sprites));
	}
}

void PropertySettingPanel::OnMultiSpriteSelected(SpriteSelection* selection)
{
	std::vector<Sprite*> sprites;
	selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty()) {
		OnSpriteSelected(NULL, true);
	} else if (sprites.size() == 1) {
		OnSpriteSelected(sprites[0], true);
	} else {
		SetPropertySetting(new MultiSpritesPropertySetting(sprites));
	}
}

void PropertySettingPanel::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			OnSpriteSelected(p->spr, p->clear);
		}
		break;
	case MSG_SELECT_SPRITE_SET:
		OnMultiSpriteSelected((SpriteSelection*)ud);
		break;
	case MSG_CLEAR_SPRITE:
		ClearSelection();
		SetPropertySetting(CreateDefaultProperty());
		break;

	case MSG_SELECT_SHAPE:
		SelectShape((Shape*)ud);
		break;
	case MSG_SELECT_SHAPE_SET:
		SelectShapeSet((ShapeSelection*)ud);
		break;
	case MSG_REMOVE_SHAPE:
		SetPropertySetting(CreateDefaultProperty());
		break;

	case MSG_CLEAR_PANEL:
		ClearSelection();
		break;
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

void PropertySettingPanel::SelectShape(Shape* shape)
{
	assert(m_stage);
	if (shape) {
		SetPropertySetting(shape->CreatePropertySetting(m_stage));
	} else {
		SetPropertySetting(CreateDefaultProperty());
	}
}

void PropertySettingPanel::SelectShapeSet(ShapeSelection* selection)
{
	std::vector<Shape*> shapes;
	selection->Traverse(FetchAllVisitor<Shape>(shapes));
	if (shapes.empty()) {
		SelectShape(NULL);
	} else if (shapes.size() == 1) {
		SelectShape(shapes[0]);
	} else {
		SelectShape(NULL);
	}
}

void PropertySettingPanel::ClearSelection()
{
	for_each(m_selection.begin(), m_selection.end(), cu::RemoveRefFonctor<Sprite>());
	m_selection.clear();
}

}