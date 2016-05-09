#ifndef _SG_SYMBOL_RENDER_H_
#define _SG_SYMBOL_RENDER_H_

namespace ee { class Sprite; class Symbol; }

namespace sg
{

class StagePanel;

class SymbolRender
{
public:
	void Init(StagePanel* stage);

	void SetGrid(ee::Sprite* grid) { m_grid = grid; }
	void SetArrow(ee::Symbol* arrow_down, ee::Symbol* arrow_right) { 
		m_arrow_down = arrow_down; 
		m_arrow_right = arrow_right;
	}
	void SetGrass(int idx, ee::Sprite* grass) { 
		if (idx >= 0 && idx < GRASS_COUNT) { m_grass[idx] = grass; }
	}
	void SetRegion(ee::Symbol* region, int size) {
		m_region = region;
		m_region_size = size;
	}

	void DrawGrass(const ee::Symbol& symbol, const sm::vec2& pos, bool is_flat) const;
	void DrawGrids(const ee::Symbol& symbol, const sm::vec2& pos, bool valid, bool is_flat) const;
	void DrawArrow(const ee::Symbol& symbol, const sm::vec2& pos) const;
	void DrawRegion(const ee::Symbol& symbol, const sm::vec2& pos);

public:
	static SymbolRender* Instance();

private:
	SymbolRender();
	~SymbolRender();

private:
	StagePanel* m_stage;

	ee::Sprite* m_grid;
	ee::Symbol *m_arrow_down, *m_arrow_right;

	static const int GRASS_COUNT = 5;
	ee::Sprite* m_grass[GRASS_COUNT];

	ee::Symbol* m_region;
	int m_region_size;

private:
	static SymbolRender* m_instance;

}; // SymbolRender

}

#endif // _SG_SYMBOL_RENDER_H_