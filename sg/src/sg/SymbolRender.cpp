#include "SymbolRender.h"
#include "StagePanel.h"
#include "SymbolInfo.h"

namespace sg
{
	
SymbolRender::SymbolRender(StagePanel* stage)
	: m_stage(stage)
{
}

void SymbolRender::DrawGrass(const d2d::ISymbol& symbol, const d2d::Vector& pos) const
{
	SymbolInfo* info = static_cast<SymbolInfo*>(symbol.getUserData());
	if (info == NULL) {
		return;
	}

	d2d::ISprite* grass = m_stage->m_grass[info->size - 1];
	grass->setTransform(pos, 0);
	d2d::SpriteDraw::drawSprite(grass);
}

void SymbolRender::DrawGrids(const d2d::ISymbol& symbol, const d2d::Vector& pos,
							 bool valid) const
{
	int row, col;
	m_stage->transCoordsToGridPos(pos, row, col);

	d2d::ISprite* grid = m_stage->m_grid;

	d2d::Colorf color = valid ? d2d::Colorf(0, 1.0f, 0, 0.8f) : d2d::Colorf(1, 0, 0, 0.8f);

	SymbolInfo* info = static_cast<SymbolInfo*>(symbol.getUserData());
	int center = (info->size >> 1);
	for (int i = 0; i < info->size; ++i) {
		for (int j = 0; j < info->size; ++j) {
			d2d::Vector pos;
			m_stage->transGridPosToCoords(row + i - center, col + j - center, pos);
			grid->setTransform(pos, grid->getAngle());
			d2d::SpriteDraw::drawSprite(grid, color);
		}
	}
}

void SymbolRender::DrawArrow(const d2d::ISymbol& symbol, const d2d::Vector& pos) const
{
	SymbolInfo* info = static_cast<SymbolInfo*>(symbol.getUserData());
	if (info == NULL) {
		return;
	}
	int r = (info->size >> 1) + 2;

	int row, col;
	m_stage->transCoordsToGridPos(pos, row, col);
	// left
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row, col - r, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_right, pos, d2d::PI);
	}
	// right
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row, col + r, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_right, pos);
	}
	// up
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row + r, col, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_down, pos, d2d::PI);
	}
	// down
	{
		d2d::Vector pos;
		m_stage->transGridPosToCoords(row - r, col, pos);
		d2d::SpriteDraw::drawSprite(m_stage->m_arrow_down, pos);
	}
}

}