#include "StagePanel.h"
#include "StageCanvas.h"
#include "ToolBarPanel.h"
#include "EditOP.h"
#include "FileIO.h"

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

	m_list.clipbox.xMin = m_list.clipbox.yMin = -100;
	m_list.clipbox.xMax = m_list.clipbox.yMax =  100;

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

	bool ret;
	if (m_toolbar->IsCurrBgLayer()) {
		ret = m_list.bgs.ResetOrder(sprite, up);
	} else {
		ret = m_list.items.ResetOrder(sprite, up);
	}
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	MultiSpritesImpl::InsertSprite(sprite, idx);

	bool ret;
	if (m_toolbar->IsCurrBgLayer()) {
		ret = m_list.bgs.Insert(sprite, idx);
	} else {
		ret = m_list.items.Insert(sprite, idx);
		m_toolbar->Enable(m_list.items.Size() == 2);
	}
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	MultiSpritesImpl::RemoveSprite(sprite);

	bool ret;
	if (m_toolbar->IsCurrBgLayer()) {
		ret = m_list.bgs.Remove(sprite);
	} else {
		ret = m_list.items.Remove(sprite);
		m_toolbar->Enable(m_list.items.Size() == 2);
	}
	if (ret) {
		SetCanvasDirty();
	}
	return ret;
}

bool StagePanel::ClearAllSprite()
{
	MultiSpritesImpl::ClearAllSprite();
	bool suc0 = m_list.bgs.Clear();
	bool suc1 = m_list.items.Clear();
	if (suc0 || suc1) {
		SetCanvasDirty();
		return true;
	} else {
		return false;
	}
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	m_list.bgs.Traverse(visitor, type, order);
	m_list.items.Traverse(visitor, type, order);
}

void StagePanel::OnSelected()
{
	m_top_pannels->library->EnableUILibrary(false);
}

void StagePanel::LoadFromFile(const char* filename)
{

}

void StagePanel::StoreToFile(const char* filename) const
{
	FileIO::Store(filename, &m_list);
}

}
}