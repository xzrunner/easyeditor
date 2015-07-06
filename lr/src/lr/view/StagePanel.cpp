#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"

#include "frame/config.h"
#include "frame/SettingCfg.h"
#include "dataset/Grids.h"
#include "dataset/Quadtree.h"
#include "dataset/Layer.h"
#include "view/LibraryPage.h"
#include "preview/PathGrid.h"
#include "preview/PathNavMesh.h"
#include "preview/PathVisibleSimple.h"

#include <easyshape.h>

namespace lr
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::PropertySettingPanel* property,
					   d2d::LibraryPanel* library,
					   d2d::ViewPanelMgr* view_panel_mgr)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(this)
	, d2d::MultiShapesImpl(this)
	, m_view_panel_mgr(view_panel_mgr)
	, m_library(library)
	, m_grids(NULL)
	, m_sindex(NULL)
	, m_pathfinding(NULL)
{
	SetDropTarget(new d2d::SpriteDropTarget(this, this, library));

	if (OPEN_GRIDS) {
		m_grids = new Grids;
		SettingCfg* cfg = SettingCfg::Instance();
		m_grids->Build(cfg->m_map_width, cfg->m_map_height);
	}

	if (TEST_QUADTREE) {
		m_sindex = new Quadtree(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
	}
	if (PATHFINDING) {
		//	m_pathfinding = new preview::PathGrid(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN), 256, 256);
		//m_pathfinding = new preview::PathNavMesh(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
		m_pathfinding = new preview::PathVisibleSimple(d2d::Rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
	}

	ArrangeSpriteImpl* arrange_impl = new ArrangeSpriteImpl(this, property, &m_chara_dirs);
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property, 
		m_view_panel_mgr, NULL, d2d::ArrangeSpriteConfig(), arrange_impl);

	m_editOP = m_arrange_op;
	m_editOP->Retain();

	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	if (m_grids) {
		delete m_grids;
	}
	if (m_pathfinding) {
		delete m_pathfinding;
	}
	m_arrange_op->Release();

	for_each(m_layers.begin(), m_layers.end(), DeletePointerFunctor<Layer>());
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	clearSprites();
	clearShapes();

	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->Release();
	}
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type/* = e_allExisting*/,
								 bool order/* = true*/) const
{
	if (SettingCfg::Instance()->m_all_layers_visible_editable ||
		type == d2d::DT_ALL ||
		type == d2d::DT_SELECTABLE) 
	{
		for (int i = 0, n = m_layers.size(); i < n; ++i) {
			m_layers[i]->TraverseSprite(visitor, order);
		}
	}
	else
	{
		for (int i = 0, n = m_layers.size(); i < n; ++i) 
		{
			Layer* layer = m_layers[i];
			if (type == d2d::DT_EDITABLE && layer->IsEditable() ||
				type == d2d::DT_VISIBLE && layer->IsVisible())
			{
				layer->TraverseSprite(visitor, order);
			}
		}
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->RemoveSprite(sprite)) {
			break;
		}
	}

	if (m_view_panel_mgr) {
		m_view_panel_mgr->RemoveSprite(sprite, this);
	}

	if (m_pathfinding) {
		m_pathfinding->DisableRegion(sprite, true);
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	GetCurrLayer()->InsertSprite(sprite);

	if (m_view_panel_mgr) {
		m_view_panel_mgr->InsertSprite(sprite, this);
	}

	if (m_sindex) {
		m_sindex->Insert(sprite);
	}
	if (m_pathfinding) {
		m_pathfinding->DisableRegion(sprite, false);
	}

	std::string filepath = sprite->getSymbol().GetFilepath();
	if (CharacterFileName::IsValidFilepath(filepath)) {
		CharacterFileName name(filepath);
		m_chara_dirs.BuildSymbolDirections(name);
	}
}

void StagePanel::clearSprites()
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->ClearSprite();
	}
}

bool StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	bool ret = false;
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->ResetOrderSprite(sprite, up)) {
			ret = true;
			break;
		}
	}

	if (m_view_panel_mgr) {
		m_view_panel_mgr->ReorderSprite(sprite, up, this);
	}

	return ret;
}

void StagePanel::traverseShapes(d2d::IVisitor& visitor, d2d::DataTraverseType type) const
{
	if (SettingCfg::Instance()->m_all_layers_visible_editable ||
		type == d2d::DT_ALL ||
		type == d2d::DT_SELECTABLE) 
	{
		for (int i = 0, n = m_layers.size(); i < n; ++i) {
			m_layers[i]->TraverseShape(visitor);
		}
	}
	else
	{
		for (int i = 0, n = m_layers.size(); i < n; ++i) 
		{
			Layer* layer = m_layers[i];
			if (type == d2d::DT_EDITABLE && layer->IsEditable() ||
				type == d2d::DT_VISIBLE && layer->IsVisible())
			{
				layer->TraverseShape(visitor);
			}
		}
	}
}

void StagePanel::removeShape(d2d::IShape* shape)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->RemoveShape(shape)) {
			break;
		}
	}
}

void StagePanel::insertShape(d2d::IShape* shape)
{
	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
	static_cast<LibraryPage*>(curr_page)->GetLayer()->InsertShape(shape);

	if (m_grids) {
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolygon(poly->GetVertices()));
		} else if (libshape::ChainShape* path = dynamic_cast<libshape::ChainShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolyline(path->GetVertices()));		
		}
	}
}

void StagePanel::clearShapes()
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->ClearShape();
	}
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

void StagePanel::SetLayers(const std::vector<Layer*>& layers)
{
	for (int i = 0, n = layers.size(); i < n; ++i) {
		layers[i]->Retain();
	}
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		m_layers[i]->Release();
	}
	m_layers = layers;
}

void StagePanel::BuildGrids(int w, int h)
{
	if (m_grids) {
		m_grids->Build(w, h);
	}
}

Layer* StagePanel::GetCurrLayer() const
{
	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
	return static_cast<LibraryPage*>(curr_page)->GetLayer();
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
// 	if (!m_editOP->IsEmpty()) {
// 		return;
// 	}
// 
// 	m_editOP->Release();
// 	if (m_editOP == m_arrange_op) {
// 		d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
// 		m_editOP = static_cast<LibraryPage*>(curr_page)->GetEditOP();
// 	} else {
// 		m_editOP = m_arrange_op;
// 	}
// 	m_editOP->Retain();
}

void StagePanel::OnKeyHook(int key_code)
{
	if (key_code >= '1' && key_code <= '8') {
		int idx = key_code - '1';
		m_library->SetCurrPage(idx);
		return;
	}

	if (key_code != 'r' && key_code != 'R') {
		return;
	}

	if (!m_editOP->IsEmpty()) {
		return;
	}

	m_editOP->Release();
	if (m_editOP == m_arrange_op) {
		d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
		m_editOP = static_cast<LibraryPage*>(curr_page)->GetExtenOP();
	} else {
		m_editOP = m_arrange_op;
	}
	m_editOP->OnActive();
	m_editOP->Retain();	
}

}