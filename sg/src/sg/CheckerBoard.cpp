#include "CheckerBoard.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "tools.h"

#include <ee/SymbolMgr.h>
#include <ee/Visitor.h>

#include <sprite2/S2_RVG.h>

#include <string>

namespace sg
{

CheckerBoard::CheckerBoard(StagePanel* stage)
	: m_stage(stage)
{
	Clear();
}

void CheckerBoard::Traverse(ee::Visitor<ee::Sprite>& visitor) const
{
	bool next;
	std::map<ee::Sprite*, sm::vec2>::const_iterator itr 
		= m_map_sprite2pos.begin();
	for ( ; itr != m_map_sprite2pos.end(); ++itr) {
		visitor.Visit(itr->first, next);
	}
}

void CheckerBoard::AddSprite(ee::Sprite* spr)
{
	int row, col;
	m_stage->TransCoordsToGridPos(spr->GetPosition(), row, col);

	SymbolExt* info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
	if (!info) {
		return;
	}

	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			m_grid[row + i - center][col + j - center] = spr;
		}
	}

	m_map_sprite2pos.insert(std::make_pair(spr, spr->GetPosition()));
}

void CheckerBoard::RemoveSprite(ee::Sprite* spr)
{
	if (!spr->GetSymbol()->GetUserData()) {
		return;
	}

	std::map<ee::Sprite*, sm::vec2>::iterator itr 
		= m_map_sprite2pos.find(spr);
	assert(itr != m_map_sprite2pos.end());

	int row, col;
	m_stage->TransCoordsToGridPos(itr->second, row, col);

	SymbolExt* info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
	if (!info) {
		return;
	}

	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			m_grid[row + i - center][col + j - center] = NULL;
		}
	}

	m_map_removed.insert(std::make_pair(itr->first, itr->second));
	m_map_sprite2pos.erase(itr);
}

void CheckerBoard::Clear()
{
	memset(&m_grid[0][0], 0, sizeof(m_grid));
}

bool CheckerBoard::IsValid(ee::Sprite* spr) const
{
	int row, col;
	m_stage->TransCoordsToGridPos(spr->GetPosition(), row, col);

	SymbolExt* info = static_cast<SymbolExt*>(spr->GetSymbol()->GetUserData());
	if (!info) {
		return false;
	}

	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			int y = row + i - center;
			int x = col + j - center;
			if (m_grid[y][x] && m_grid[y][x] != spr) {
				return false;
			}
		}
	}

	return true;
}

bool CheckerBoard::IsValid(const ee::Symbol& sym, const sm::vec2& pos) const
{
	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);

	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
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
				sm::vec2 pos;
				m_stage->TransGridPosToCoords(i, j, pos);
				s2::RVG::SetColor(s2::Color(0, 0, 0));
				s2::RVG::Circle(pos, 10, true);
			}
		}
	}
}

bool CheckerBoard::SetCachedPos(ee::Sprite* spr) const
{
	std::map<ee::Sprite*, sm::vec2>::const_iterator itr 
		= m_map_removed.find(spr);
	if (itr != m_map_removed.end()) {
		spr->SetPosition(itr->second);
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

			ee::Sprite* spr = m_grid[i][j];
			const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
			std::string filepath = sym->GetFilepath();
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

			if (filepath == sym->GetFilepath()) {
				continue;
			}

			SymbolExt* old = static_cast<SymbolExt*>(sym->GetUserData());
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