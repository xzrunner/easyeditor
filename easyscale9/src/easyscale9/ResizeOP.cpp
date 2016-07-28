#include "ResizeOP.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

#include <ee/panel_msg.h>

namespace escale9
{

ResizeOP::ResizeOP(StagePanel* stage, ToolbarPanel* toolbar, ResizeCMPT* resizeCmpt)
	: escale9::ResizeBaseOP(stage, stage->GetStageImpl(), NULL)
	, m_stage(stage)
	, m_toolbar(toolbar)
	, m_resizeCmpt(resizeCmpt)
{
}

bool ResizeOP::OnMouseLeftDown(int x, int y)
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::OnMouseLeftDown(x, y);
}

bool ResizeOP::OnMouseLeftUp(int x, int y)
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::OnMouseLeftUp(x, y);
}

bool ResizeOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	Symbol* symbol = dynamic_cast<Symbol*>(m_stage->getPatchSymbol());
	if (m_status != e_null && symbol)
	{
		float width, height;

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

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
			height = symbol->GetSize().Size().y;
		}
		else if (type == e_3GridVer)
		{
			width = symbol->GetSize().Size().x;
			height = fabs(pos.y)*2;
		}
		else
		{
			return true;
		}

		m_resizeCmpt->setSize(width, height);
		symbol->ResizeScale9(width, height);

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool ResizeOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) return true;

	m_toolbar->setComposeOP(false);
	m_stage->rebuildPatchSymbol();

	return false;
}

bool ResizeOP::OnDraw() const
{
	Symbol* symbol = m_stage->getPatchSymbol();
	if (!symbol) return false;

	escale9::ResizeBaseOP::setSymbol(symbol);
	return escale9::ResizeBaseOP::OnDraw();
}

} // escale9