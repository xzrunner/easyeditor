#include "ResizeOP.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

namespace escale9
{

ResizeOP::ResizeOP(StagePanel* stage, ToolbarPanel* toolbar, ResizeCMPT* resizeCmpt)
	: escale9::ResizeBaseOP(stage, NULL)
	, m_stage(stage)
	, m_toolbar(toolbar)
	, m_resizeCmpt(resizeCmpt)
{
}

bool ResizeOP::onMouseLeftDown(int x, int y)
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::onMouseLeftDown(x, y);
}

bool ResizeOP::onMouseLeftUp(int x, int y)
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::onMouseLeftUp(x, y);
}

bool ResizeOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	Symbol* symbol = dynamic_cast<Symbol*>(m_stage->getPatchSymbol());
	if (m_status != e_null && symbol)
	{
		float width, height;

		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);

		Scale9Type type = symbol->GetScale9Data().GetType();
		if (type == e_9Grid || 
			type == e_9GridHollow ||
			type == e_6GridUpper)
		{
			width = fabs(pos.x)*2;
			height = fabs(pos.y)*2;
		}
		else if (type == e_3GridHor)
		{
			width = fabs(pos.x)*2;
			height = symbol->getSize().yLength();
		}
		else if (type == e_3GridVer)
		{
			width = symbol->getSize().xLength();
			height = fabs(pos.y)*2;
		}
		else
		{
			return true;
		}

		m_resizeCmpt->setSize(width, height);
		symbol->ResizeScale9(width, height);

		m_stage->Refresh();
	}

	return false;
}

bool ResizeOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) return true;

	m_toolbar->setComposeOP(false);
	m_stage->rebuildPatchSymbol();

	return false;
}

bool ResizeOP::onDraw() const
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::onDraw();
}

} // escale9