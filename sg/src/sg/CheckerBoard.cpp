#include "CheckerBoard.h"
#include "StagePanel.h"
#include "SymbolInfo.h"

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
			m_grid[row + i - center][col + j - center] = true;
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
			m_grid[row + i - center][col + j - center] = false;
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
	return IsValid(sprite->getSymbol(), sprite->getPosition());
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

void CheckerBoard::SetCachedPos(d2d::ISprite* sprite) const
{
	std::map<d2d::ISprite*, d2d::Vector>::const_iterator itr 
		= m_mapRemoved.find(sprite);
	if (itr != m_mapRemoved.end())
	{
		sprite->setTransform(itr->second, sprite->getAngle());
	}
}

}