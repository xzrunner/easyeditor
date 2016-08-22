#include "ResizeOP.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"

#include <ee/panel_msg.h>

#include <sprite2/Scale9.h>

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
	Symbol* sym = m_stage->getPatchSymbol();
	if (!sym) return false;

	escale9::ResizeBaseOP::setSymbol(sym);
	return escale9::ResizeBaseOP::OnMouseLeftDown(x, y);
}

bool ResizeOP::OnMouseLeftUp(int x, int y)
{
	Symbol* sym = m_stage->getPatchSymbol();
	if (!sym) return false;

	escale9::ResizeBaseOP::setSymbol(sym);
	return escale9::ResizeBaseOP::OnMouseLeftUp(x, y);
}

bool ResizeOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	Symbol* sym = dynamic_cast<Symbol*>(m_stage->getPatchSymbol());
	if (m_status != e_null && sym)
	{
		float width, height;

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

		s2::SCALE9_TYPE type = sym->GetScale9().GetType();
		if (type == s2::S9_9GRID || 
			type == s2::S9_9GRID_HOLLOW ||
			type == s2::S9_6GRID_UPPER)
		{
			width = fabs(pos.x)*2;
			height = fabs(pos.y)*2;
		}
		else if (type == s2::S9_3GRID_HORI)
		{
			width = fabs(pos.x)*2;
			height = sym->GetBounding().Size().y;
		}
		else if (type == s2::S9_3GRID_VERT)
		{
			width = sym->GetBounding().Size().x;
			height = fabs(pos.y)*2;
		}
		else
		{
			return true;
		}

		m_resizeCmpt->setSize(width, height);
		sym->Resize(width, height);

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
	Symbol* sym = m_stage->getPatchSymbol();
	if (!sym) return false;

	escale9::ResizeBaseOP::setSymbol(sym);
	return escale9::ResizeBaseOP::OnDraw();
}

} // escale9