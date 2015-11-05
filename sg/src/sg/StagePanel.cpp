#include "StagePanel.h"
#include "StageCanvas.h"
#include "BuildingCfg.h"
#include "SymbolExt.h"
#include "SpriteExt.h"
#include "tools.h"

namespace sg
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, m_library(library)
	, m_is_flat(false)
	, m_building_cfg(this, library)
	, m_checkboard(this)
{
	SetCanvas(new StageCanvas(this));
	m_row = m_col = m_edge = 0;

	m_base_level = 1;

	d2d::InsertSpriteSJ::Instance()->Register(this);
	d2d::RemoveSpriteSJ::Instance()->Register(this);
	d2d::ClearSpriteSJ::Instance()->Register(this);
}

StagePanel::~StagePanel()
{
	d2d::InsertSpriteSJ::Instance()->UnRegister(this);
	d2d::RemoveSpriteSJ::Instance()->UnRegister(this);
	d2d::ClearSpriteSJ::Instance()->UnRegister(this);
}

void StagePanel::Notify(int sj_id, void* ud)
{
	d2d::MultiSpritesImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			Insert(p->spr);
		}
		break;
	case d2d::MSG_REMOVE_SPRITE:
		Remove((d2d::ISprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void StagePanel::TransCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const
{
	d2d::Vector p;
	if (m_is_flat) {
		p = pos;
	} else {
		p = StageCanvas::TransToFlatView(pos);		
	}
	col = std::max(0.0f, std::min(p.x / m_edge, (float)(m_col - 1)));
	row = std::max(0.0f, std::min(p.y / m_edge, (float)(m_row - 1)));
}

void StagePanel::TransGridPosToCoords(int row, int col, d2d::Vector& pos) const
{
	pos.x = m_edge * col + m_edge * 0.5f;
	pos.y = m_edge * row + m_edge * 0.5f;
	if (!m_is_flat) {
		pos = StageCanvas::TransToBirdView(pos);
	}
}

void StagePanel::TransCoordsToGridPosNew(const d2d::Vector& pos, int& row, int& col) const
{
	d2d::Vector p;
	if (m_is_flat) {
		p = pos;
	} else {
		p = StageCanvas::TransToFlatView(pos);		
	}

	col = (int)(p.x / m_edge);
	row = (int)(p.y / m_edge);
	assert(col >= 0 && col < COL && row >= 0 && row < ROW);
	col = COL - 1 - col;
	row = ROW - 1 - row;
}

void StagePanel::TransGridPosToCoordsNew(int row, int col, d2d::Vector& pos) const
{
	pos.x = m_edge * (COL - 1 - col) + m_edge * 0.5f;
	pos.y = m_edge * (ROW - 1 - row) + m_edge * 0.5f;
	if (!m_is_flat) {
		pos = StageCanvas::TransToBirdView(pos);
	}
}

void StagePanel::UpdateAllSpritesLocation()
{
	std::vector<d2d::ISprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		s->SetTransform(FixSpriteLocation(s->GetPosition()), s->GetAngle());
	}
}

void StagePanel::SetPerspective(bool is_flat) 
{ 
	if (m_is_flat == is_flat) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		int row, col;
		TransCoordsToGridPos(sprite->GetPosition(), row, col);
		m_is_flat = !m_is_flat;

		d2d::Vector pos;
		TransGridPosToCoords(row, col, pos);
		m_is_flat = !m_is_flat;

		sprite->SetTransform(pos, sprite->GetAngle());
	}

 	m_is_flat = is_flat; 
}

void StagePanel::ChangeSelectedSpritesLevel(bool up)
{
	std::vector<d2d::ISprite*> sprites;
	GetSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		SpriteExt* spr_info = static_cast<SpriteExt*>(sprite->GetUserData());
		SymbolExt* symbol_info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
		assert(spr_info && symbol_info);

		if (spr_info->level == 1 && !up ||
			spr_info->level == symbol_info->building->levels.size() && up) {
			continue;
		}

		spr_info->level = up ? spr_info->level + 1 : spr_info->level - 1;
		std::string new_filepath = symbol_info->building->levels[spr_info->level - 1].res_snapshoot_path;
		d2d::ISymbol* new_symbol = d2d::SymbolMgr::Instance()->FetchSymbol(new_filepath);
		if (new_symbol != &sprite->GetSymbol()) {
			SymbolExt* new_symbol_info = static_cast<SymbolExt*>(new_symbol->GetUserData());
			new_symbol_info->remain--;
			symbol_info->remain++;
			sprite->SetSymbol(new_symbol);
		}

// 		//////////////////////////////////////////////////////////////////////////
// 
// 		const BuildingCfg::Item* pItem;
// 		if (up && symbol_info->level != symbol_info->building->levels.size() - 1) {
// 			pItem = &symbol_info->building->levels[symbol_info->level + 1];
// 		} else if (!up && symbol_info->level != 0) {
// 			pItem = &symbol_info->building->levels[symbol_info->level - 1];
// 		} else {
// 			continue;
// 		}
// 
// 		if (pItem->town_hall_level > m_base_level) {
// 			return;
// 		}
// 
// 		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(pItem->res_snapshoot_path);
// 		if (symbol) {
// 			if (symbol->GetUserData() == NULL) 
// 			{
// 				SymbolExt* new_info = new SymbolExt;
// 				new_info->size = symbol_info->size;
// 				new_info->remain = symbol_info->remain;
// 				new_info->wall_type = symbol_info->wall_type;
// 				new_info->level = pItem->level;
// 				new_info->building = pItem->building;
// 				symbol->SetUserData(new_info);
// 			}
// 			sprite->setSymbol(symbol);
// 		}
// 		symbol->Release();
	}
}

d2d::Vector StagePanel::FixSpriteLocation(const d2d::Vector& pos) const
{
	int row, col;
	TransCoordsToGridPos(pos, row, col);

	d2d::Vector ret;
	TransGridPosToCoords(row, col, ret);
	return ret;
}

void StagePanel::ChangeSymbolRemain(d2d::ISprite* sprite, bool increase) const
{
	SymbolExt* info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
	if (!info) {
		return;
	}

	if (increase) {
		++info->remain;
	} else {
		--info->remain;
	}
	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(sprite->GetSymbol());
	symbol.SetInfo(wxString::FromDouble(info->remain).ToStdString());
	m_library->Refresh(true);
}

void StagePanel::Insert(d2d::ISprite* spr)
{
	spr->SetTransform(FixSpriteLocation(spr->GetPosition()), spr->GetAngle());

	if (!spr->GetSymbol().GetUserData()) {
//		d2d::SpritesPanelImpl::InsertSprite(spr);
		return;
	}

	if (!spr->GetPosition().isValid()) {
		return;
	}

	if (!m_checkboard.IsValid(spr)) {
		bool fixed = m_checkboard.SetCachedPos(spr);
		if (!fixed) {
			return;
		}
	}

	if (!spr->GetUserData()) {
		SpriteExt* ext = new SpriteExt;
		ext->level = ((SymbolExt*)(spr->GetSymbol().GetUserData()))->level;
		spr->SetUserData(ext);
	}

//	bool ret = d2d::SpritesPanelImpl::InsertSprite(spr);
	ChangeSymbolRemain(spr, false);
	m_checkboard.AddSprite(spr);

	if (IsSymbolWall(*spr)) {
		m_checkboard.ResetWall();
	}
}

void StagePanel::Remove(d2d::ISprite* spr)
{
	bool reset_wall = IsSymbolWall(*spr);

	ChangeSymbolRemain(spr, true);
//	bool ret = SpritesPanelImpl::RemoveSprite(spr);
	m_checkboard.RemoveSprite(spr);

	if (reset_wall) {
		m_checkboard.ResetWall();
	}
}

void StagePanel::Clear()
{
	std::vector<d2d::ISprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	bool ret = !sprites.empty();
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ChangeSymbolRemain(sprites[i], true);
	}
	m_checkboard.Clear();

//	SpritesPanelImpl::ClearAllSprite();
}

}