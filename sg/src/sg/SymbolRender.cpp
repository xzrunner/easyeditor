#include "SymbolRender.h"
#include "StagePanel.h"
#include "SymbolExt.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/Matrix.h>
#include <ee/EE_RVG.h>
#include <ee/color_config.h>
#include <ee/SpriteRenderer.h>

namespace sg
{
	
SymbolRender* SymbolRender::m_instance = NULL;

SymbolRender::SymbolRender()
	: m_stage(NULL)
	, m_region(NULL)
	, m_region_size(0)
{
	m_grid = NULL;
	m_arrow_down = m_arrow_right = NULL;
	for (int i = 0; i < GRASS_COUNT; ++i) {
		m_grass[i] = NULL;
	}
}

SymbolRender::~SymbolRender()
{
	m_grid->Release();
	m_arrow_down->Release();
	m_arrow_right->Release();
	for (int i = 0; i < GRASS_COUNT; ++i) {
		m_grass[i]->Release();
	}
}

void SymbolRender::Init(StagePanel* stage)
{
	m_stage = stage;
}

void SymbolRender::DrawGrass(const ee::Symbol& symbol, 
							 const ee::Vector& pos, 
							 bool is_flat) const
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}

	ee::Vector p = pos;
	if (info->size % 2 == 0)
	{
		int row, col;
		m_stage->TransCoordsToGridPos(pos, row, col);

		ee::Vector fixed;
		m_stage->TransGridPosToCoords(row - 1, col - 1, fixed);

		p = (pos + fixed) * 0.5f;
	}

	ee::Sprite* grass = m_grass[info->size - 1];
	if (is_flat)
	{
		ee::Rect r = grass->GetSymbol().GetSize();

		float half_edge = info->size * EDGE * 0.5f;
		ee::RVG::Color(ee::LIGHT_GREEN);
		ee::RVG::Rect(p, half_edge, half_edge, true);
	}
	else
	{
		grass->SetTransform(p, 0);
		ee::SpriteRenderer::Instance()->Draw(grass);
	}
}

void SymbolRender::DrawGrids(const ee::Symbol& symbol, 
							 const ee::Vector& pos, 
							 bool valid,
							 bool is_flat) const
{
	if (!symbol.GetUserData()) {
		return;
	}

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);

	ee::Colorf color = valid ? ee::Colorf(0, 1.0f, 0, 0.8f) : ee::Colorf(1, 0, 0, 0.8f);

	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	int center = (info->size >> 1);
	if (is_flat)
	{
		ee::Vector pos;
		m_stage->TransGridPosToCoords(row, col, pos);

		float half_edge = info->size * EDGE * 0.5f;
		ee::RVG::Color(color);
 		ee::RVG::Rect(pos, half_edge, half_edge, true);
	}
	else
	{
		for (int i = 0; i < info->size; ++i) {
			for (int j = 0; j < info->size; ++j) {
				ee::Vector pos;
				m_stage->TransGridPosToCoords(row + i - center, col + j - center, pos);
				m_grid->SetTransform(pos, m_grid->GetAngle());
				ee::ColorTrans ct;
				ct.multi = color;
				ee::SpriteRenderer::Instance()->Draw(m_grid, NULL, ee::SpriteTrans(ee::Matrix(), ct));
			}
		}
	}
}

void SymbolRender::DrawArrow(const ee::Symbol& symbol, 
							 const ee::Vector& pos) const
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}
	int r = (info->size >> 1) + 2;

	ee::SpriteRenderer* rd = ee::SpriteRenderer::Instance();

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);
	// left
	{
		ee::Vector pos;
		m_stage->TransGridPosToCoords(row, col - r, pos);
		rd->Draw(m_arrow_down, ee::Matrix(), pos, 0, -1);
	}
	// right
	{
		ee::Vector pos;
		m_stage->TransGridPosToCoords(row, col + r, pos);
		rd->Draw(m_arrow_right, ee::Matrix(), pos);
	}
	// up
	{
		ee::Vector pos;
		m_stage->TransGridPosToCoords(row + r, col, pos);
		rd->Draw(m_arrow_right, ee::Matrix(), pos, 0, -1);
	}
	// down
	{
		ee::Vector pos;
		m_stage->TransGridPosToCoords(row - r, col, pos);
		rd->Draw(m_arrow_down, ee::Matrix(), pos);
	}
}

void SymbolRender::DrawRegion(const ee::Symbol& symbol, const ee::Vector& pos)
{
	SymbolExt* info = static_cast<SymbolExt*>(symbol.GetUserData());
	if (info == NULL) {
		return;
	}

	int max_reg, min_reg;
	bool find = m_stage->GetBuildingCfg().QueryAttackRegion(info->building->name, max_reg, min_reg);
	if (!find) {
		return;
	}

	float s = (float)(max_reg) / m_region_size;
	ee::SpriteRenderer* rd = ee::SpriteRenderer::Instance();
	ee::ColorTrans ct;
	ct.multi = ee::Colorf(1, 0, 0);
	rd->Draw(m_region, ee::SpriteTrans(ee::Matrix(), ct), pos, 0, s, s, 0, 0);
	if (min_reg != 0) {
		float s = (float)(min_reg) / m_region_size;
		rd->Draw(m_region, ee::Matrix(), pos, 0, s, s);
	}
}

SymbolRender* SymbolRender::Instance()
{
	if (!m_instance)
	{
		m_instance = new SymbolRender();
	}
	return m_instance;
}

}