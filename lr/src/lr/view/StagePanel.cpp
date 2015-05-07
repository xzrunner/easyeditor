#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"

#include "frame/config.h"
#include "dataset/Quadtree.h"
#include "dataset/Layer.h"
#include "view/LibraryPage.h"
#include "preview/PathGrid.h"
#include "preview/PathNavMesh.h"
#include "preview/PathVisibleSimple.h"

namespace lr
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(this)
	, m_symbols_cfg(this, library)
	, m_library(library)
	, m_viewlist(NULL)
	, m_sindex(NULL)
	, m_pathfinding(NULL)
{
	if (TEST_QUADTREE) {
		m_sindex = new Quadtree(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
	}
	if (PATHFINDING) {
		//	m_pathfinding = new preview::PathGrid(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN), 256, 256);
		//m_pathfinding = new preview::PathNavMesh(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
		m_pathfinding = new preview::PathVisibleSimple(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
	}

	m_paste_op = new d2d::PasteSymbolOP(this, this, library);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);

	m_editOP = m_paste_op;
	m_editOP->Retain();

	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	if (m_pathfinding) {
		delete m_pathfinding;
	}
	m_paste_op->Release();
	m_arrange_op->Release();
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::TraverseType type/* = e_allExisting*/,
								 bool order/* = true*/) const
{
	GetCurrLayer()->traverse(visitor, type, order);
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	GetCurrLayer()->remove(sprite);
	m_viewlist->remove(sprite);

	if (m_pathfinding) {
		m_pathfinding->DisableRegion(sprite, true);
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	GetCurrLayer()->insert(sprite);
	m_viewlist->insert(sprite);

	if (m_sindex) {
		m_sindex->Insert(sprite);
	}
	if (m_pathfinding) {
		m_pathfinding->DisableRegion(sprite, false);
	}
}

void StagePanel::clearSprites()
{
	GetCurrLayer()->clear();
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	GetCurrLayer()->resetOrder(sprite, up);
	m_viewlist->reorder(sprite, up);
}

void StagePanel::DebugDraw() const
{
	if (m_sindex) {
		m_sindex->DebugDraw();
	}
	if (m_pathfinding) {
		m_pathfinding->DebugDraw();
	}
}

void StagePanel::Pathfinding(const d2d::Vector& start, const d2d::Vector& end)
{
	if (m_pathfinding) {
		m_pathfinding->QueryRoute(start, end);
	}
}

void StagePanel::PointQuery(const d2d::Vector& pos)
{
	if (m_sindex) {
		m_sindex->SelectNode(pos);
	}
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
	if (!m_editOP->IsEmpty()) {
		return;
	}

	m_editOP->Release();
	if (m_editOP == m_arrange_op) {
		m_editOP = m_paste_op;
	} else {
		m_editOP = m_arrange_op;
	}
	m_editOP->Retain();
}

Layer* StagePanel::GetCurrLayer() const
{
	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
	return static_cast<LibraryPage*>(curr_page)->GetLayer();
}

}