#include "SymbolRender.h"
#include "StagePanel.h"
#include "SymbolExt.h"

#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/color_config.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/S2_RVG.h>

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
	m_grid->RemoveReference();
	m_arrow_down->RemoveReference();
	m_arrow_right->RemoveReference();
	for (int i = 0; i < GRASS_COUNT; ++i) {
		m_grass[i]->RemoveReference();
	}
}

void SymbolRender::Init(StagePanel* stage)
{
	m_stage = stage;
}

void SymbolRender::DrawGrass(const ee::Symbol& sym, 
							 const sm::vec2& pos, 
							 bool is_flat) const
{
	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
	if (info == NULL) {
		return;
	}

	sm::vec2 p = pos;
	if (info->size % 2 == 0)
	{
		int row, col;
		m_stage->TransCoordsToGridPos(pos, row, col);

		sm::vec2 fixed;
		m_stage->TransGridPosToCoords(row - 1, col - 1, fixed);

		p = (pos + fixed) * 0.5f;
	}

	ee::Sprite* grass = m_grass[info->size - 1];
	if (is_flat)
	{
		sm::rect r = grass->GetSymbol()->GetBounding();

		float half_edge = info->size * EDGE * 0.5f;
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Rect(p, half_edge, half_edge, true);
	}
	else
	{
		grass->SetPosition(p);
		grass->SetAngle(0);
		ee::SpriteRenderer::Draw(grass);
	}
}

void SymbolRender::DrawGrids(const ee::Symbol& sym, 
							 const sm::vec2& pos, 
							 bool valid,
							 bool is_flat) const
{
	if (!sym.GetUserData()) {
		return;
	}

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);

	s2::Color color = valid ? s2::Color(0, 255, 0, 204) : s2::Color(255, 0, 0, 204);

	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
	int center = (info->size >> 1);
	if (is_flat)
	{
		sm::vec2 pos;
		m_stage->TransGridPosToCoords(row, col, pos);

		float half_edge = info->size * EDGE * 0.5f;
		s2::RVG::SetColor(color);
 		s2::RVG::Rect(pos, half_edge, half_edge, true);
	}
	else
	{
		for (int i = 0; i < info->size; ++i) {
			for (int j = 0; j < info->size; ++j) {
				sm::vec2 pos;
				m_stage->TransGridPosToCoords(row + i - center, col + j - center, pos);
				m_grid->SetPosition(pos);
				s2::RenderColor ct;
				ct.mul = color;
				ee::SpriteRenderer::Draw(m_grid, NULL, s2::RenderParams(sm::mat4(), ct));
			}
		}
	}
}

void SymbolRender::DrawArrow(const ee::Symbol& sym, 
							 const sm::vec2& pos) const
{
	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
	if (info == NULL) {
		return;
	}
	int r = (info->size >> 1) + 2;

	int row, col;
	m_stage->TransCoordsToGridPos(pos, row, col);
	// left
	{
		sm::vec2 pos;
		m_stage->TransGridPosToCoords(row, col - r, pos);
		ee::SpriteRenderer::Draw(m_arrow_down, sm::mat4(), pos, 0, -1);
	}
	// right
	{
		sm::vec2 pos;
		m_stage->TransGridPosToCoords(row, col + r, pos);
		ee::SpriteRenderer::Draw(m_arrow_right, sm::mat4(), pos);
	}
	// up
	{
		sm::vec2 pos;
		m_stage->TransGridPosToCoords(row + r, col, pos);
		ee::SpriteRenderer::Draw(m_arrow_right, sm::mat4(), pos, 0, -1);
	}
	// down
	{
		sm::vec2 pos;
		m_stage->TransGridPosToCoords(row - r, col, pos);
		ee::SpriteRenderer::Draw(m_arrow_down, sm::mat4(), pos);
	}
}

void SymbolRender::DrawRegion(const ee::Symbol& sym, const sm::vec2& pos)
{
	SymbolExt* info = static_cast<SymbolExt*>(sym.GetUserData());
	if (info == NULL) {
		return;
	}

	int max_reg, min_reg;
	bool find = m_stage->GetBuildingCfg().QueryAttackRegion(info->building->name, max_reg, min_reg);
	if (!find) {
		return;
	}

	float s = (float)(max_reg) / m_region_size;
	s2::RenderColor ct;
	ct.mul = s2::Color(255, 0, 0);
	ee::SpriteRenderer::Draw(m_region, s2::RenderParams(sm::mat4(), ct), pos, 0, s, s, 0, 0);
	if (min_reg != 0) {
		float s = (float)(min_reg) / m_region_size;
		ee::SpriteRenderer::Draw(m_region, sm::mat4(), pos, 0, s, s);
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