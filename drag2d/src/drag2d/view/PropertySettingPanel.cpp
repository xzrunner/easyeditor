#include "PropertySettingPanel.h"
#include "IPropertySetting.h"

#include "common/visitors.h"
#include "dataset/ISprite.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/SpriteSelection.h"
#include "view/ShapeSelection.h"

#include "message/subject_id.h"
#include "message/SelectSpriteSJ.h"
#include "message/SelectSpriteSetSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSJ.h"
#include "message/SelectShapeSJ.h"
#include "message/SelectShapeSetSJ.h"
#include "message/RemoveShapeSJ.h"

#include <algorithm>

namespace d2d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_setting(NULL)
	, m_stage(NULL)
{
	InitLayout();

	SetBackgroundColour(wxColour(229, 229, 229));

	m_subjects.push_back(SelectSpriteSJ::Instance());
	m_subjects.push_back(SelectSpriteSetSJ::Instance());
	m_subjects.push_back(InsertSpriteSJ::Instance());
	m_subjects.push_back(RemoveSpriteSJ::Instance());
	m_subjects.push_back(ClearSJ::Instance());

	m_subjects.push_back(SelectShapeSJ::Instance());
	m_subjects.push_back(SelectShapeSetSJ::Instance());
	m_subjects.push_back(RemoveShapeSJ::Instance());

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

PropertySettingPanel::~PropertySettingPanel()
{
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}

	delete m_setting;
}

void PropertySettingPanel::Notify(int sj_id, void* ud)
{
	switch (SELECT_SPRITE)
	{
	case SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			OnSpriteSelected(p->spr, p->clear);
		}
		break;
	case SELECT_SPRITE_SET:
		OnMultiSpriteSelected((SpriteSelection*)ud);
		break;
	case INSERT_SPRITE:
		SetPropertySetting(CreateDefaultProperty());
		break;
	case REMOVE_SPRITE:
		SetPropertySetting(CreateDefaultProperty());
		break;
	case CLEAR:
		SetPropertySetting(CreateDefaultProperty());
		break;

	case SELECT_SHAPE:
		SelectShape((IShape*)ud);
		break;
	case SELECT_SHAPE_SET:
		SelectShapeSet((ShapeSelection*)ud);
		break;
	case REMOVE_SHAPE:
		SetPropertySetting(CreateDefaultProperty());
		break;
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

void PropertySettingPanel::OnSpriteSelected(ISprite* spr, bool clear)
{
	if (spr) {
		std::set<ISprite*>::iterator itr = m_selection.find(spr);
		if (itr != m_selection.end()) {
			return;
		}
	}

	if (clear) {
		for_each(m_selection.begin(), m_selection.end(), ReleaseObjectFunctor<ISprite>());
		m_selection.clear();
	}
	if (spr) {
		spr->Retain();
		m_selection.insert(spr);
	}

	assert(m_stage);
	if (m_selection.size() == 1) {
		if (spr) {
			SetPropertySetting(spr->CreatePropertySetting(m_stage));
		} else {
			SetPropertySetting(CreateDefaultProperty());
		}
	} else if (m_selection.size() > 1) {
		std::vector<ISprite*> sprites;
		std::copy(m_selection.begin(), m_selection.end(), back_inserter(sprites));
		SetPropertySetting(new MultiSpritesPropertySetting(m_stage, sprites));
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

void PropertySettingPanel::SelectShape(IShape* shape)
{
	assert(m_stage);
	if (shape) {
		SetPropertySetting(shape->createPropertySetting(m_stage));
	} else {
		SetPropertySetting(CreateDefaultProperty());
	}
}

void PropertySettingPanel::SelectShapeSet(ShapeSelection* selection)
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

} // d2d