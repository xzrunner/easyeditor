#include "StagePanel.h"
#include "StageCanvas.h"
#include "ToolBarPanel.h"
#include "EditOP.h"

#include "dataset/TopPannels.h"
#include "view/LibraryPanel.h"
#include "view/ToolbarPanel.h"

namespace eui
{
namespace list
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
	, m_top_pannels(top_pannels)
{
	top_pannels->view_panel_mgr.AddSpritePanel(this);

	SetEditOP(new EditOP(this, top_pannels->property, &top_pannels->view_panel_mgr));
	SetCanvas(new StageCanvas(this));

	d2d::LibraryPanel* library = top_pannels->library->GetRawLibrary();
	SetDropTarget(new d2d::SpriteDropTarget(this, GetStageImpl(), library));
	library->SetCanvas(GetCanvas());

	top_pannels->property->SetEditPanel(GetStageImpl());

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, &m_list, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);
}

void StagePanel::Clear()
{
}

bool StagePanel::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	MultiSpritesImpl::ReorderSprite(sprite, up);

	bool ret = m_list.GetItems().ResetOrder(sprite, up);
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	MultiSpritesImpl::InsertSprite(sprite, idx);

	bool ret = m_list.GetItems().Insert(sprite, idx);
	m_toolbar->Enable(m_list.GetItems().Size() == 2);
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	MultiSpritesImpl::RemoveSprite(sprite);

	bool ret = m_list.GetItems().Remove(sprite);
	m_toolbar->Enable(m_list.GetItems().Size() == 2);
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::ClearAllSprite()
{
	MultiSpritesImpl::ClearAllSprite();
	if (m_list.GetItems().Clear()) {
		SetCanvasDirty();
		return true;
	} else {
		return false;
	}
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	m_list.GetItems().Traverse(visitor, type, order);
}

void StagePanel::OnSelected()
{
	m_top_pannels->library->EnableUILibrary(false);
}

void StagePanel::LoadFromFile(const char* filename)
{
	m_list.LoadFromFile(filename);
	SetCanvasDirty();
}

void StagePanel::StoreToFile(const char* filename) const
{
	m_list.StoreToFile(filename);
}

}
}