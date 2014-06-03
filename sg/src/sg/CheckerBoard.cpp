#include "CheckerBoard.h"
#include "StagePanel.h"
#include "SymbolInfo.h"
#include "tools.h"

#include <string>

namespace sg
{

CheckerBoard::CheckerBoard(StagePanel* stage)
	: m_stage(stage)
{
	Clear();
}

void CheckerBoard::AddSprite(d2d::ISprite* sprite)
{
	int row, col;
	m_stage->transCoordsToGridPos(sprite->getPosition(), row, col);

	SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			m_grid[row + i - center][col + j - center] = sprite;
		}
	}

	m_mapSprite2Pos.insert(std::make_pair(sprite, sprite->getPosition()));
}

void CheckerBoard::RemoveSprite(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, d2d::Vector>::iterator itr 
		= m_mapSprite2Pos.find(sprite);
	assert(itr != m_mapSprite2Pos.end());

	int row, col;
	m_stage->transCoordsToGridPos(itr->second, row, col);

	SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			m_grid[row + i - center][col + j - center] = NULL;
		}
	}

	m_mapRemoved.insert(std::make_pair(itr->first, itr->second));
	m_mapSprite2Pos.erase(itr);
}

void CheckerBoard::Clear()
{
	memset(&m_grid[0][0], 0, sizeof(m_grid));
}

bool CheckerBoard::IsValid(d2d::ISprite* sprite) const
{
	int row, col;
	m_stage->transCoordsToGridPos(sprite->getPosition(), row, col);

	SymbolInfo* info = static_cast<SymbolInfo*>(sprite->getSymbol().getUserData());
	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			int y = row + i - center;
			int x = col + j - center;
			if (m_grid[y][x] && m_grid[y][x] != sprite) {
				return false;
			}
		}
	}

	return true;
}

bool CheckerBoard::IsValid(const d2d::ISymbol& symbol, const d2d::Vector& pos) const
{
	int row, col;
	m_stage->transCoordsToGridPos(pos, row, col);

	SymbolInfo* info = static_cast<SymbolInfo*>(symbol.getUserData());
	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			if (m_grid[row + i - center][col + j - center]) {
				return false;
			}
		}
	}

	return true;
}

void CheckerBoard::DebugDraw() const
{
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COL; ++j) {
			if (m_grid[i][j]) {
				d2d::Vector pos;
				m_stage->transGridPosToCoords(i, j, pos);
				d2d::PrimitiveDraw::drawCircle(pos, 10, true, 2, d2d::Colorf(0, 0, 0));
			}
		}
	}
}

bool CheckerBoard::SetCachedPos(d2d::ISprite* sprite) const
{
	std::map<d2d::ISprite*, d2d::Vector>::const_iterator itr 
		= m_mapRemoved.find(sprite);
	if (itr != m_mapRemoved.end()) {
		sprite->setTransform(itr->second, sprite->getAngle());
		return true;
	} else {
		return false;
	}
}

void CheckerBoard::ResetWall()
{
	for (int i = ROW - 1; i > 0; --i) {
		for (int j = 0; j < ROW - 1; ++j) {
			bool curr = m_grid[i][j] && IsSymbolWall(m_grid[i][j]);
			bool right = m_grid[i][j+1] && IsSymbolWall(m_grid[i][j+1]);
			bool up = m_grid[i-1][j] && IsSymbolWall(m_grid[i-1][j]);

			if (!curr) {
				continue;
			}

			const d2d::ISymbol& symbol = m_grid[i][j]->getSymbol();
			wxString filepath = symbol.getFilepath();
			int s = filepath.find("lv") + 2;
			int e = filepath.find('_', s-1);
			if (e != -1) {
				filepath = filepath.substr(0, e) + ".png";
			}
			int dot = filepath.find_last_of(".");

			if (!right && !up) {
				;
			} else if (right && up) {
				filepath = filepath.insert(dot, "_3");
			} else if (right) {
				filepath = filepath.insert(dot, "_2");
			} else if (up) {
				filepath = filepath.insert(dot, "_1");
			}

			if (filepath == symbol.getFilepath()) {
				continue;
			}

			SymbolInfo* old = static_cast<SymbolInfo*>(symbol.getUserData());
			SymbolInfo* info = new SymbolInfo;
			*info = *old;
			
			d2d::ISymbol* new_s = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			new_s->setUserData(info);
			m_grid[i][j]->setSymbol(new_s);
		}
	}
}

}