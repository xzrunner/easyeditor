#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "ArrangeSpriteImpl.h"
#include "typedef.h"

#include "frame/config.h"
#include "frame/SettingCfg.h"
#include "dataset/Grids.h"
#include "dataset/Quadtree.h"
#include "dataset/Layer.h"
#include "dataset/TagCfg.h"
#include "view/LibraryPage.h"
#include "preview/PathGrid.h"
#include "preview/PathNavMesh.h"
#include "preview/PathVisibleSimple.h"

#include <ee/SpriteDropTarget.h>
#include <ee/ArrangeSpriteOP.h>
#include <ee/sprite_msg.h>
#include <ee/shape_msg.h>
#include <ee/std_functor.h>
#include <ee/Config.h>
#include <ee/LibraryPanel.h>
#include <ee/SettingData.h>
#include <ee/subject_id.h>
#include <ee/panel_msg.h>

#include <easyshape.h>
#include <easyparticle3d.h>

#include <algorithm>

namespace lr
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::PropertySettingPanel* property,
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
	, ee::MultiShapesImpl()
	, m_library(library)
	, m_grids(NULL)
	, m_sindex(NULL)
	, m_pathfinding(NULL)
	, m_enable_update(true)
{
	SetDropTarget(new ee::SpriteDropTarget(GetStageImpl(), library));

	if (OPEN_GRIDS) {
		m_grids = new Grids;
		SettingCfg* cfg = SettingCfg::Instance();
		m_grids->Build(cfg->m_map_width, cfg->m_map_height);
	}

	if (TEST_QUADTREE) {
		m_sindex = new Quadtree(sm::rect(sm::vec2(0, 0), MAP_EDGE_LEN, MAP_EDGE_LEN));
	}
	if (PATHFINDING) {
		//	m_pathfinding = new preview::PathGrid(sm::rect(MAP_EDGE_LEN, MAP_EDGE_LEN), 256, 256);
		//m_pathfinding = new preview::PathNavMesh(sm::rect(MAP_EDGE_LEN, MAP_EDGE_LEN));
		m_pathfinding = new preview::PathVisibleSimple(sm::rect(sm::vec2(0, 0), MAP_EDGE_LEN, MAP_EDGE_LEN));
	}

	ArrangeSpriteImpl* arrange_impl = new ArrangeSpriteImpl(library, property, this, &m_chara_dirs);
	m_arrange_op = new ee::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, property, 
		NULL, ee::ArrangeSpriteConfig(), arrange_impl);

	SetEditOP(m_arrange_op);
	SetCanvas(new StageCanvas(this));

	ee::MultiSpritesImpl::RegistSubject(ee::ReorderSpriteSJ::Instance());
	ee::MultiSpritesImpl::RegistSubject(ee::ReorderSpriteMostSJ::Instance());
	ee::MultiSpritesImpl::RegistSubject(ee::InsertSpriteSJ::Instance());
	ee::MultiSpritesImpl::RegistSubject(ee::RemoveSpriteSJ::Instance());
	ee::MultiSpritesImpl::RegistSubject(ee::ClearSpriteSJ::Instance());

	ee::MultiShapesImpl::RegistSubject(ee::RemoveShapeSJ::Instance());
	ee::MultiShapesImpl::RegistSubject(ee::InsertShapeSJ::Instance());
	ee::MultiShapesImpl::RegistSubject(ee::ClearShapeSJ::Instance());
}

StagePanel::~StagePanel()
{
	if (m_grids) {
		delete m_grids;
	}
	if (m_pathfinding) {
		delete m_pathfinding;
	}
	m_arrange_op->RemoveReference();

	for_each(m_layers.begin(), m_layers.end(), ee::DeletePointerFunctor<Layer>());
}

// void StagePanel::Clear()
// {
// 	ee::EditPanel::Clear();
// 	ClearSprite();
// 	ClearShape();
// 
// 	for (int i = 0, n = m_layers.size(); i < n; ++i) {
// 		m_layers[i]->RemoveReference();
// 	}
// }

bool StagePanel::UpdateStage()
{
	if (!m_enable_update) {
		return false;
	}

	bool ret = false;

	for (int i = 0, n = m_layers.size(); i < n; ++i) {
		if (m_layers[i]->Update()) {
			ret = true;
		}
	}

	if (!ee::Config::Instance()->GetSettings().visible_spr_update) {
		ret = false;
	}

	return ret;
}

void StagePanel::TraverseSprites(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type/* = e_allExisting*/,
								 bool order/* = true*/) const
{
	if (SettingCfg::Instance()->m_all_layers_visible_editable ||
		type == ee::DT_ALL ||
		type == ee::DT_SELECTABLE) 
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
			if (type == ee::DT_EDITABLE && layer->IsEditable() ||
				type == ee::DT_VISIBLE && layer->IsVisible())
			{
				layer->TraverseSprite(visitor, type, order);
			}
		}
	}
}

void StagePanel::TraverseShapes(ee::Visitor<ee::Shape>& visitor, ee::DataTraverseType type) const
{
	if (SettingCfg::Instance()->m_all_layers_visible_editable ||
		type == ee::DT_ALL ||
		type == ee::DT_SELECTABLE) 
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
			if (type == ee::DT_EDITABLE && layer->IsEditable() ||
				type == ee::DT_VISIBLE && layer->IsVisible())
			{
				layer->TraverseShape(visitor);
			}
		}
	}
}

void StagePanel::EnableObserve(bool enable)
{
	GetCanvas()->EnableObserve(enable);
	MultiSpritesImpl::EnableObserve(enable);
	MultiShapesImpl::EnableObserve(enable);
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

void StagePanel::Pathfinding(const sm::vec2& start, const sm::vec2& end)
{
	if (m_pathfinding) {
		m_pathfinding->QueryRoute(start, end);
	}
}

void StagePanel::PointQuery(const sm::vec2& pos)
{
	if (m_sindex) {
		m_sindex->SelectNode(pos);
	}
}

void StagePanel::SetLayers(const std::vector<Layer*>& layers)
{
	if (m_layers.empty()) {
		for_each(layers.begin(), layers.end(), cu::AddRefFonctor<Layer>());
		m_layers = layers;
		return;
	}

	assert(m_layers.size() >= layers.size());
	for (int i = 0, n = layers.size(); i < n; ++i) {
		m_layers[i]->RemoveReference();
		layers[i]->AddReference();
		m_layers[i] = layers[i];
	}
}

void StagePanel::BuildGrids(int w, int h)
{
	if (m_grids) {
		m_grids->Build(w, h);
	}
}

s2::Color& StagePanel::GetScreenMultiColor()
{
	StageCanvas* canvas = static_cast<StageCanvas*>(GetCanvas());
	return canvas->GetMultiColor();
}

s2::Color& StagePanel::GetScreenAddColor()
{
	StageCanvas* canvas = static_cast<StageCanvas*>(GetCanvas());
	return canvas->GetAddColor();	
}

void StagePanel::OnMouseHook(wxMouseEvent& event)
{
	if (event.RightDown()) {
		ChangeEditOP();
	}	
}

void StagePanel::ChangeEditOP()
{
// 	if (!m_editop->IsEmpty()) {
// 		return;
// 	}
// 
// 	m_editop->RemoveReference();
// 	if (m_editop == m_arrange_op) {
// 		ee::LibraryPage* curr_page = m_library->GetCurrPage();
// 		m_editop = static_cast<LibraryPage*>(curr_page)->GetEditOP();
// 	} else {
// 		m_editop = m_arrange_op;
// 	}
// 	m_editop->AddReference();
}

void StagePanel::OnKeyHook(int key_code)
{
	if (key_code >= '1' && key_code <= '9') {
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

	ee::LibraryPage* curr_page = m_library->GetCurrPage();
	SetEditOP(static_cast<LibraryPage*>(curr_page)->GetNextEditOP());
	GetEditOP()->OnActive();

// 	m_edit_op->RemoveReference();
// 	ee::LibraryPage* curr_page = m_library->GetCurrPage();
// 	m_edit_op = static_cast<LibraryPage*>(curr_page)->GetNextEditOP();
// 	m_edit_op->OnActive();
// 	m_edit_op->AddReference();	
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_REORDER_SPRITE:
		{
			ee::ReorderSpriteSJ::Params* p = (ee::ReorderSpriteSJ::Params*)ud;
			ReorderSprite(p->spr, p->up);
		}
		break;
	case ee::MSG_REORDER_SPRITE_MOST:
		{
			ee::ReorderSpriteMostSJ::Params* p = (ee::ReorderSpriteMostSJ::Params*)ud;
			ReorderSpriteMost(p->spr, p->up);
		}
		break;
	case ee::MSG_SORT_SPRITES:
		{
			std::vector<ee::Sprite*>& sprs = *(std::vector<ee::Sprite*>*)ud;
			SortSprites(sprs);
		}
		break;
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			InsertSprite(p->spr, p->idx);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		RemoveSprite((ee::Sprite*)ud);
		break;
	case ee::MSG_CLEAR_SPRITE:
		ClearSprite();
		break;

	case ee::MSG_REMOVE_SHAPE:
		RemoveShape((ee::Shape*)ud);
		break;
	case ee::MSG_INSERT_SHAPE:
		InsertShape((ee::Shape*)ud);
		break;
	case ee::MSG_CLEAR_SHAPE:
		ClearShape();
		break;
	}
}

void StagePanel::ReorderSprite(ee::Sprite* spr, bool up)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->ResetOrderSprite(spr, up)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			break;
		}
	}
}

void StagePanel::ReorderSpriteMost(ee::Sprite* spr, bool up)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->ResetOrderSpriteMost(spr, up)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			break;
		}
	}
}

void StagePanel::SortSprites(std::vector<ee::Sprite*>& sprs)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->SortSrites(sprs)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			break;
		}
	}
}

void StagePanel::InsertSprite(ee::Sprite* spr, int idx)
{
	if (dynamic_cast<eparticle3d::Sprite*>(spr) && spr->GetTag().empty()) {
		std::string tag = TOP_LAYER_TAG;
		spr->SetTag(tag  + ";");
	}

	// tag
	std::string tag = TagCfg::Instance()->Query(dynamic_cast<ee::Symbol*>(spr->GetSymbol()));
	if (spr->GetTag().find(tag) == std::string::npos) {
		spr->SetTag(tag + spr->GetTag());
	}

	Layer* layer = static_cast<LibraryPage*>(m_library->GetCurrPage())->GetLayer();
	if (layer->InsertSprite(spr, idx)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	if (m_sindex) {
		m_sindex->Insert(spr);
	}
	if (m_pathfinding) {
		m_pathfinding->DisableRegion(spr, false);
	}

	std::string filepath = dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath();
	if (CharacterFileName::IsValidFilepath(filepath)) {
		CharacterFileName name(filepath);
		m_chara_dirs.BuildSymbolDirections(name);
	}
}

void StagePanel::RemoveSprite(ee::Sprite* spr)
{
	for (int i = 0, n = m_layers.size(); i < n; ++i)
	{
		Layer* layer = m_layers[i];
		if (layer->RemoveSprite(spr)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}
}

void StagePanel::RemoveShape(ee::Shape* shape)
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
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::InsertShape(ee::Shape* shape)
{
	ee::LibraryPage* curr_page = m_library->GetCurrPage();
	bool dirty = static_cast<LibraryPage*>(curr_page)->GetLayer()->InsertShape(shape);

	if (m_grids) {
		if (eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolygon(poly->GetVertices()));
		} else if (eshape::ChainShape* path = dynamic_cast<eshape::ChainShape*>(shape)) {
			m_grids->SetDebbugDrawGrids(m_grids->IntersectPolyline(path->GetVertices()));		
		}
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}