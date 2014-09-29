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

	void SetGrid(d2d::ISprite* grid) { m_grid = grid; }
	void SetArrow(d2d::ISymbol* arrow_down, d2d::ISymbol* arrow_right) { 
		m_arrow_down = arrow_down; 
		m_arrow_right = arrow_right;
	}
	void SetGrass(int idx, d2d::ISprite* grass) { 
		if (idx >= 0 && idx < GRASS_COUNT) { m_grass[idx] = grass; }
	}

	void DrawGrass(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;
	void DrawGrids(const d2d::ISymbol& symbol, const d2d::Vector& pos, bool valid) const;
	void DrawArrow(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;

public:
	static SymbolRender* Instance();

private:
	SymbolRender();
	~SymbolRender();

private:
	StagePanel* m_stage;

	d2d::ISprite* m_grid;
	d2d::ISymbol *m_arrow_down, *m_arrow_right;

	static const int GRASS_COUNT = 5;
	d2d::ISprite* m_grass[GRASS_COUNT];

private:
	static SymbolRender* m_instance;

}; // SymbolRender

}

#endif // _SG_SYMBOL_RENDER_H_