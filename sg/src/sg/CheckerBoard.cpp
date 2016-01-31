#include "CheckerBoard.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "tools.h"

#include <string>

namespace sg
{

CheckerBoard::CheckerBoard(StagePanel* stage)
	: m_stage(stage)
{
	Clear();
}

void CheckerBoard::AddSprite(ee::Sprite* sprite)
{
	int row, col;
	m_stage->TransCoordsToGridPos(sprite->GetPosition(), row, col);

	SymbolExt* info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
	if (!info) {
		return;
	}

	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			m_grid[row + i - center][col + j - center] = sprite;
		}
	}

	m_mapSprite2Pos.insert(std::make_pair(sprite, sprite->GetPosition()));
}

void CheckerBoard::RemoveSprite(ee::Sprite* sprite)
{
	if (!sprite->GetSymbol().GetUserData()) {
		return;
	}

	std::map<ee::Sprite*, ee::Vector>::iterator itr 
		= m_mapSprite2Pos.find(sprite);
	assert(itr != m_mapSprite2Pos.end());

	int row, col;
	m_stage->TransCoordsToGridPos(itr->second, row, col);

	SymbolExt* info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
	if (!info) {
		return;
	}

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

bool CheckerBoard::IsValid(ee::Sprite* sprite) const
{
	int row, col;
	m_stage->TransCoordsToGridPos(sprite->GetPosition(), row, col);

	SymbolExt* info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
	if (!info) {
		return false;
	}

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

bool CheckerBoard::IsValid(const ee::Symbol& symbol, const ee::Vector& pos) const
{
	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);

	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (!info) {
		return false;
	}

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
				ee::Vector pos;
				m_stage->TransGridPosToCoords(i, j, pos);
				ee::PrimitiveDraw::DrawCircle(pos, 10, true, 2, ee::Colorf(0, 0, 0));
			}
		}
	}
}

bool CheckerBoard::SetCachedPos(ee::Sprite* sprite) const
{
	std::map<ee::Sprite*, ee::Vector>::const_iterator itr 
		= m_mapRemoved.find(sprite);
	if (itr != m_mapRemoved.end()) {
		sprite->SetTransform(itr->second, sprite->GetAngle());
		return true;
	} else {
		return false;
	}
}

void CheckerBoard::ResetWall()
{
	for (int i = 0; i < ROW - 1; ++i) {
		for (int j = 0; j < COL - 1; ++j) {
			bool curr = m_grid[i][j] && IsSymbolWall(*m_grid[i][j]);
			bool right = m_grid[i][j+1] && IsSymbolWall(*m_grid[i][j+1]);
			bool up = m_grid[i+1][j] && IsSymbolWall(*m_grid[i+1][j]);

			if (!curr) {
				continue;
			}

			ee::Sprite* sprite = m_grid[i][j];
			const ee::Symbol& symbol = sprite->GetSymbol();
			std::string filepath = symbol.GetFilepath();
			int s = filepath.find("lv") + 2;
			int e = filepath.find('_', s-1);
			if (e != -1) {
				filepath = filepath.substr(0, e) + ".png";
			}
			int dot = filepath.find_last_of(".");

			int wall_type = 0;
			if (!right && !up) {
				;
			} else if (right && up) {
				wall_type = 3;
				filepath = filepath.insert(dot, "_3");
			} else if (right) {
				wall_type = 2;
				filepath = filepath.insert(dot, "_2");
			} else if (up) {
				wall_type = 1;
				filepath = filepath.insert(dot, "_1");
			}

			if (filepath == symbol.GetFilepath()) {
				continue;
			}

			SymbolExt* old = static_cast<SymbolExt*>(symbol.GetUserData());
			SymbolExt* info = new SymbolExt;
			*info = *old;
			info->wall_type = wall_type;
			
			ee::Symbol* new_s = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			new_s->SetUserData(info);
			m_grid[i][j]->SetSymbol(new_s);
		}
	}
}

}