#ifndef _SG_SYMBOL_RENDER_H_
#define _SG_SYMBOL_RENDER_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class SymbolRender
{
public:
	void Init(StagePanel* stage);

	void SetGrid(d2d::Sprite* grid) { m_grid = grid; }
	void SetArrow(d2d::Symbol* arrow_down, d2d::Symbol* arrow_right) { 
		m_arrow_down = arrow_down; 
		m_arrow_right = arrow_right;
	}
	void SetGrass(int idx, d2d::Sprite* grass) { 
		if (idx >= 0 && idx < GRASS_COUNT) { m_grass[idx] = grass; }
	}
	void SetRegion(d2d::Symbol* region, int size) {
		m_region = region;
		m_region_size = size;
	}

	void DrawGrass(const d2d::Symbol& symbol, const d2d::Vector& pos, bool is_flat) const;
	void DrawGrids(const d2d::Symbol& symbol, const d2d::Vector& pos, bool valid, bool is_flat) const;
	void DrawArrow(const d2d::Symbol& symbol, const d2d::Vector& pos) const;
	void DrawRegion(const d2d::Symbol& symbol, const d2d::Vector& pos);

public:
	static SymbolRender* Instance();

private:
	SymbolRender();
	~SymbolRender();

private:
	StagePanel* m_stage;

	d2d::Sprite* m_grid;
	d2d::Symbol *m_arrow_down, *m_arrow_right;

	static const int GRASS_COUNT = 5;
	d2d::Sprite* m_grass[GRASS_COUNT];

	d2d::Symbol* m_region;
	int m_region_size;

private:
	static SymbolRender* m_instance;

}; // SymbolRender

}

#endif // _SG_SYMBOL_RENDER_H_