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
	, d2d::SpritesPanelImpl(this, library)
	, m_library(library)
	, m_is_flat(false)
	, m_building_cfg(this, library)
	, m_checkboard(this)
{
	m_canvas = new StageCanvas(this);
	m_row = m_col = m_edge = 0;

	m_base_level = 1;
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	bool reset_wall = IsSymbolWall(*sprite);

	ChangeSymbolRemain(sprite, true);
	SpritesPanelImpl::removeSprite(sprite);
	m_checkboard.RemoveSprite(sprite);

	if (reset_wall) {
		m_checkboard.ResetWall();
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	sprite->setTransform(FixSpriteLocation(sprite->getPosition()), sprite->getAngle());

	if (!sprite->getSymbol().getUserData()) {
		d2d::SpritesPanelImpl::insertSprite(sprite);
		return;
	}

	if (!sprite->getPosition().isValid()) {
		return;
	}

	if (!m_checkboard.IsValid(sprite)) {
		bool fixed = m_checkboard.SetCachedPos(sprite);
		if (!fixed) {
			return;
		}
	}

	if (!sprite->getUserData()) {
		SpriteExt* ext = new SpriteExt;
		ext->level = ((SymbolExt*)(sprite->getSymbol().getUserData()))->level;
		sprite->setUserData(ext);
	}

	d2d::SpritesPanelImpl::insertSprite(sprite);
	ChangeSymbolRemain(sprite, false);
	m_checkboard.AddSprite(sprite);

	if (IsSymbolWall(*sprite)) {
		m_checkboard.ResetWall();
	}
}

void StagePanel::clearSprites()
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		ChangeSymbolRemain(sprites[i], true);
	}
	m_checkboard.Clear();

	SpritesPanelImpl::clearSprites();
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
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* s = sprites[i];
		s->setTransform(FixSpriteLocation(s->getPosition()), s->getAngle());
	}
}

void StagePanel::SetPerspective(bool is_flat) 
{ 
	if (m_is_flat == is_flat) {
		return;
	}

	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		int row, col;
		TransCoordsToGridPos(sprite->getPosition(), row, col);
		m_is_flat = !m_is_flat;

		d2d::Vector pos;
		TransGridPosToCoords(row, col, pos);
		m_is_flat = !m_is_flat;

		sprite->setTransform(pos, sprite->getAngle());
	}

 	m_is_flat = is_flat; 
}

//void StagePanel::changeSpritesLevel(bool up)
//{
//	std::vector<d2d::ISprite*> sprites;
//	getSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
//	for (int i = 0, n = sprites.size(); i < n; ++i)
//	{
//		d2d::ISprite* sprite = sprites[i];
//		wxString filepath = sprite->getSymbol().getFilepath();
//		if (!filepath.Contains("lv")) {
//			continue;
//		}
//
//		int s = filepath.find("lv") + 2;
//		int e = filepath.find('_', s-1);
//		if (s == -1 || e == -1) {
//			continue;
//		}
//
//		wxString sLevel = filepath.substr(s, e - s);
//		double level;
//		sLevel.ToDouble(&level);
//		level = up ? level + 1 : level - 1;
//
//		wxString newfilepath = filepath.substr(0, s) + wxString::FromDouble(level) + filepath.substr(e);
//
//		if (!wxFileName::FileExists(newfilepath)) {
//			continue;
//		}
//
//		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(newfilepath);
//		if (symbol) {
//			sprite->setSymbol(symbol);
//		}
//		symbol->Release();
//	}
//}

void StagePanel::ChangeSelectedSpritesLevel(bool up)
{
	std::vector<d2d::ISprite*> sprites;
	getSpriteSelection()->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		SymbolExt* info = static_cast<SymbolExt*>(sprite->getSymbol().getUserData());
		if (info == NULL || info->building->levels.size() == 1) {
			continue;
		}

		const BuildingCfg::Item* pItem;
		if (up && info->level != info->building->levels.size() - 1) {
			pItem = &info->building->levels[info->level + 1];
		} else if (!up && info->level != 0) {
			pItem = &info->building->levels[info->level - 1];
		} else {
			continue;
		}

		if (pItem->town_hall_level > m_base_level) {
			return;
		}

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(pItem->res_snapshoot_path);
		if (symbol) {
			if (symbol->getUserData() == NULL) 
			{
				SymbolExt* new_info = new SymbolExt;
				new_info->size = info->size;
				new_info->remain = info->remain;
				new_info->wall_type = info->wall_type;
				new_info->level = pItem->level;
				new_info->building = pItem->building;
				symbol->setUserData(new_info);
			}
			sprite->setSymbol(symbol);
		}
		symbol->Release();
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
	SymbolExt* info = static_cast<SymbolExt*>(sprite->getSymbol().getUserData());
	if (!info) {
		return;
	}

	if (increase) {
		++info->remain;
	} else {
		--info->remain;
	}
	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(sprite->getSymbol());
	symbol.setInfo(wxString::FromDouble(info->remain));
	m_library->Refresh();
}

}