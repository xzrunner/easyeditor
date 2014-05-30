#ifndef _SG_SYMBOL_RENDER_H_
#define _SG_SYMBOL_RENDER_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class SymbolRender
{
public:
	SymbolRender(StagePanel* stage);

	void DrawGrass(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;
	void DrawGrids(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;
	void DrawArrow(const d2d::ISymbol& symbol, const d2d::Vector& pos) const;

private:
	StagePanel* m_stage;

}; // SymbolRender

}

#endif // _SG_SYMBOL_RENDER_H_