#include "ResizeBaseOP.h"
#include "ResizeAtomicOP.h"
#include "Symbol.h"

namespace escale9
{

ResizeBaseOP::ResizeBaseOP(d2d::EditPanel* editPanel, Symbol* symbol)
	: d2d::ZoomViewOP(editPanel, true)
	, m_symbol(symbol)
	, m_status(e_null)
{
}

bool ResizeBaseOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos = m_stage->transPosScreenToProject(x, y);

	const float hw = m_symbol->getSize().xLength() * 0.5f,
		hh = m_symbol->getSize().yLength() * 0.5f;
	if (d2d::Math::isPointInRect(m_firstPos, d2d::Vector(-hw, -hh), REGION, REGION))
		m_status = e_leftlow;
	else if (d2d::Math::isPointInRect(m_firstPos, d2d::Vector(hw, -hh), REGION, REGION))
		m_status = e_rightlow;
	else if (d2d::Math::isPointInRect(m_firstPos, d2d::Vector(hw, hh), REGION, REGION))
		m_status = e_rightup;
	else if (d2d::Math::isPointInRect(m_firstPos, d2d::Vector(-hw, hh), REGION, REGION))
		m_status = e_leftup;

	return false;
}

bool ResizeBaseOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y)) return true;

	if (m_status != e_null)
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		const d2d::Vector src(fabs(m_firstPos.x)*2, fabs(m_firstPos.y)*2),
			dst(fabs(pos.x)*2, fabs(pos.y)*2);
		m_stage->addHistoryOP(new ResizeAtomicOP(m_symbol, src, dst));
	}

	m_status = e_null;

	m_stage->Refresh();

	return false;
}

bool ResizeBaseOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_status != e_null)
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		m_symbol->ResizeScale9(fabs(pos.x)*2, fabs(pos.y)*2);
		m_stage->Refresh();
	}

	return false;
}

bool ResizeBaseOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	d2d::SpriteDraw::drawSprite(m_symbol);

	const float hw = m_symbol->getSize().xLength() * 0.5f,
		hh = m_symbol->getSize().yLength() * 0.5f;
	const float r = REGION;
	d2d::PrimitiveDraw::rect(d2d::Vector(-hw, -hh), r, r, m_style);
	d2d::PrimitiveDraw::rect(d2d::Vector( hw, -hh), r, r, m_style);
	d2d::PrimitiveDraw::rect(d2d::Vector( hw,  hh), r, r, m_style);
	d2d::PrimitiveDraw::rect(d2d::Vector(-hw,  hh), r, r, m_style);

	return false;
}
} // escale9