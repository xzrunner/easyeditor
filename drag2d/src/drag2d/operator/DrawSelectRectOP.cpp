#include "DrawSelectRectOP.h"

#include "render/PrimitiveDraw.h"
#include "view/IStageCanvas.h"
#include "render/style_config.h"

namespace d2d
{

DrawSelectRectOP::DrawSelectRectOP(EditPanel* editPanel, bool bOpenRightTap)
	: ZoomViewOP(editPanel, true, bOpenRightTap)
{
	m_first_pos.setInvalid();
	m_last_pos.setInvalid();
}

bool DrawSelectRectOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_first_pos = m_stage->TransPosScrToProj(x, y);

	return false;
}

bool DrawSelectRectOP::OnMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_first_pos.setInvalid();
	m_last_pos.setInvalid();

	return false;
}

bool DrawSelectRectOP::OnMouseDrag(int x, int y)
{
	if (ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_first_pos.isValid())
	{
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		m_stage->SetCanvasDirty();
	}

	return false;
}

bool DrawSelectRectOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	if (!m_first_pos.isValid() || !m_last_pos.isValid()) {
		return false;
	}

	if (m_last_pos.x > m_first_pos.x)
	{
		PrimitiveDraw::rect(m_first_pos, m_last_pos, SELECT_ALL);
		PrimitiveDraw::rect(m_first_pos, m_last_pos, SELECT_BOUND);
	}
	else
	{
		PrimitiveDraw::rect(m_first_pos, m_last_pos, SELECT_PART);
		PrimitiveDraw::rect(m_first_pos, m_last_pos, SELECT_BOUND);
	}

	return false;
}

bool DrawSelectRectOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_first_pos.setInvalid();
	m_last_pos.setInvalid();

	return false;
}

} // d2d