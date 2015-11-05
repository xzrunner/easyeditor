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
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
	, d2d::MultiShapesImpl(GetStageImpl())
	, m_library(library)
	, m_grids(NULL)
	, m_sindex(NULL)
	, m_pathfinding(NULL)
	, m_enable_update(true)
{
	SetDropTarget(new d2d::SpriteDropTarget(GetStageImpl(), library));

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
	m_arrange_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, d2d::ArrangeSpriteConfig(), arrange_impl);

	SetEditOP(m_arrange_op);
	SetCanvas(new StageCanvas(this));

	d2d::MultiSpritesImpl::m_subjects.push_back(d2d::ReorderSpriteSJ::Instance());
	d2d::MultiSpritesImpl::m_subjects.push_back(d2d::ReorderSpriteMostSJ::Instance());
	d2d::MultiSpritesImpl::m_subjects.push_back(d2d::InsertSpriteSJ::Instance());
	d2d::MultiSpritesImpl::m_subjects.push_back(d2d::RemoveSpriteSJ::Instance());
	d2d::MultiSpritesImpl::m_subjects.push_back(d2d::ClearSpriteSJ::Instance());
	for (int i = 0, n = d2d::MultiSpritesImpl::m_subjects.size(); i < n; ++i) {
		d2d::MultiSpritesImpl::m_subjects[i]->Register((d2d::MultiSpritesImpl*)this);
	}

	d2d::MultiShapesImpl::m_subjects.push_back(d2d::RemoveShapeSJ::Instance());
	d2d::MultiShapesImpl::m_subjects.push_back(d2d::InsertShapeSJ::Instance());
	d2d::MultiShapesImpl::m_subjects.push_back(d2d::ClearShapeSJ::Instance());
	for (int i = 0, n = d2d::MultiShapesImpl::m_subjects.size(); i < n; ++i) {
		d2d::MultiShapesImpl::m_subjects[i]->Register((d2d::MultiShapesImpl*)this);
	}
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

	for (int i = 0, n = d2d::MultiSpritesImpl::m_subjects.size(); i < n; ++i) {
		d2d::MultiSpritesImpl::m_subjects[i]->UnRegister((d2d::MultiSpritesImpl*)this);
	}
	for (int i = 0, n = d2d::MultiShapesImpl::m_subjects.size(); i < n; ++i) {
		d2d::MultiShapesImpl::m_subjects[i]->UnRegister((d2d::MultiShapesImpl*)this);
	}
}

// void StagePanel::Clear()
// {
// 	d2d::EditPanel::Clear();
// 	ClearSprite();
// 	ClearShape();
// 
// 	for (int i = 0, n = m_layers.size(); i < n; ++i) {
// 		m_layers[i]->Release();
// 	}
// }

bool StagePanel::Update(int version)
{
	if (!m_enable_update) {
		return false;
	}

	bool ret = false;

	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		if (m_layers[i]->Update(version)) {
			ret = true;
		}
	}

	if (!d2d::Config::Instance()->GetSettings().visible_spr_update) {
		ret = false;
	}

	return ret;
}

void StagePanel::Notify(int sj_id, void* ud)
{
	d2d::MultiSpritesImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_REORDER_SPRITE:
		{
			d2d::ReorderSpriteSJ::Params* p = (d2d::ReorderSpriteSJ::Params*)ud;
			ReorderSprite(p->spr, p->up);
		}
		break;
	case d2d::MSG_REORDER_SPRITE_MOST:
		{
			d2d::ReorderSpriteMostSJ::Params* p = (d2d::ReorderSpriteMostSJ::Params*)ud;
			ReorderSpriteMost(p->spr, p->up);
		}
		break;
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			InsertSprite(p->spr);
		}
		break;
	case d2d::MSG_REMOVE_SPRITE:
		RemoveSprite((d2d::ISprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		ClearSprite();
		break;

	case d2d::MSG_REMOVE_SHAPE:
		RemoveShape((d2d::IShape*)ud);
		break;
	case d2d::MSG_INSERT_SHAPE:
		InsertShape((d2d::IShape*)ud);
		break;
	case d2d::MSG_CLEAR_SHAPE:
		ClearShape();
		break;
	}
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type/* = e_allExisting*/,
								 bool order/* = true*/) const
{
	if (SettingCfg::Instance()->m_all_layers_visible_editable ||
		type == d2d::DT_ALL ||
		type == d2d::DT_SELECTABLE) 
	{
		for (int i = 0, n = m_layers.size(); i < n; ++i) {
			m_layers[i]->TraverseSprite(visitor, type, order);
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
				layer->TraverseSprite(visitor, type, order);
			}
		}
	}
}

void StagePanel::TraverseShapes(d2d::IVisitor& visitor, d2d::DataTraverseType type) const
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

	if (!GetEditOP()->IsEmpty()) {
		return;
	}

	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
	SetEditOP(static_cast<LibraryPage*>(curr_page)->GetNextEditOP());
	GetEditOP()->OnActive();

// 	m_edit_op->Release();
// 	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
// 	m_edit_op = static_cast<LibraryPage*>(curr_page)->GetNextEditOP();
// 	m_edit_op->OnActive();
// 	m_edit_op->Retain();	
}

void StagePanel::ReorderSprite(d2d::ISprite* spr, bool up)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->ResetOrderSprite(spr, up)) {
			SetCanvasDirty();
			break;
		}
	}
}

void StagePanel::ReorderSpriteMost(d2d::ISprite* spr, bool up)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->ResetOrderSpriteMost(spr, up)) {
			SetCanvasDirty();
			break;
		}
	}
}

void StagePanel::InsertSprite(d2d::ISprite* spr)
{
// 	idx = m_view_panel_mgr->GetSelection() + 1;
// 	d2d::MultiSpritesImpl::InsertSprite(sprite, idx);
// 
// 	Layer* layer = static_cast<LibraryPage*>(m_library->GetCurrPage())->GetLayer();
// 	bool ret = layer->InsertSprite(sprite, idx);
// 
// 	if (m_view_panel_mgr) {
// 		m_view_panel_mgr->InsertSprite(sprite, this, idx);
// 	}
// 
// 	if (m_sindex) {
// 		m_sindex->Insert(sprite);
// 	}
// 	if (m_pathfinding) {
// 		m_pathfinding->DisableRegion(sprite, false);
// 	}
// 
// 	std::string filepath = sprite->GetSymbol().GetFilepath();
// 	if (CharacterFileName::IsValidFilepath(filepath)) {
// 		CharacterFileName name(filepath);
// 		m_chara_dirs.BuildSymbolDirections(name);
// 	}
// 
// 	if (ret) {
// 		SetCanvasDirty();
// 	}
// 
// 	return ret;
}

void StagePanel::RemoveSprite(d2d::ISprite* spr)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->RemoveSprite(spr)) {
			SetCanvasDirty();
			break;
		}
	}

	if (m_pathfinding) {
		m_pathfinding->DisableRegion(spr, true);
	}
}

void StagePanel::ClearSprite()
{
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		if (m_layers[i]->ClearSprite()) {
			SetCanvasDirty();
		}
	}
}

void StagePanel::RemoveShape(d2d::IShape* shape)
{
	bool dirty = false;
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->RemoveShape(shape)) {
			dirty = true;
			break;
		}
	}
	if (dirty) {
		SetCanvasDirty();
	}
}

void StagePanel::InsertShape(d2d::IShape* shape)
{
	d2d::ILibraryPage* curr_page = m_library->GetCurrPage();
	bool dirty = static_cast<LibraryPage*>(curr_page)->GetLayer()->InsertShape(shape);

	if (m_grids) {
		if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolygon(poly->GetVertices()));
		} else if (libshape::ChainShape* path = dynamic_cast<libshape::ChainShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolyline(path->GetVertices()));		
		}
	}

	if (dirty) {
		SetCanvasDirty();
	}
}

void StagePanel::ClearShape()
{
	bool dirty = false;
	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		if (m_layers[i]->ClearShape()) {
			dirty = true;
		}
	}
	if (dirty) {
		SetCanvasDirty();
	}
}

}