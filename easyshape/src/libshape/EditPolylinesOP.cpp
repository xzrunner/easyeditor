#include "EditPolylinesOP.h"
#include "EditPolylinesCMPT.h"
#include "ChainShape.h"

namespace libshape
{

EditPolylinesOP::EditPolylinesOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
								 d2d::PropertySettingPanel* propertyPanel, EditPolylinesCMPT* cmpt)
	: d2d::SelectShapesOP(editPanel, shapesImpl, propertyPanel, cmpt)
	, m_cmpt(cmpt)
	, m_bDirty(false)
{
	m_lastPos.setInvalid();

	clearBuffer();
}

bool EditPolylinesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::SelectShapesOP::onMouseLeftDown(x, y)) return true;

	if (!m_firstPos.isValid())
		m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	else
		m_lastPos.setInvalid();

	return false;
}

bool EditPolylinesOP::onMouseLeftUp(int x, int y)
{
	if (d2d::SelectShapesOP::onMouseLeftUp(x, y)) return true;

	if (m_bDirty)
		m_selection->Traverse(UpdateChainVisitor());

	clearBuffer();
	m_selection->Traverse(UpdateBufferVisitor(m_simplifyBuffer));

	return false;
}

bool EditPolylinesOP::onMouseDrag(int x, int y)
{
	if (d2d::SelectShapesOP::onMouseDrag(x, y)) return true;

	if (m_lastPos.isValid())
	{
		d2d::Vector currPos = m_editPanel->transPosScreenToProject(x, y);
		d2d::Vector offset = currPos - m_lastPos;
		m_selection->Traverse(OffsetVisitor(offset));
		m_lastPos = currPos;

		m_bDirty = true;
		m_editPanel->Refresh();
	}

	return false;
}

bool EditPolylinesOP::onDraw() const
{
	if (d2d::SelectShapesOP::onDraw()) return true;

	std::map<ChainShape*, ChainShape*>::const_iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
	{
		itr->second->draw(d2d::Colorf(0.8f, 0.8f, 0.2f));
		d2d::PrimitiveDraw::drawCircles(itr->second->getVertices(), d2d::Settings::ctlPosSize, true, 2, d2d::Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditPolylinesOP::clear()
{
	if (d2d::SelectShapesOP::clear()) return true;

	clearBuffer();

	return false;
}

void EditPolylinesOP::simplify()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
	{
		std::vector<d2d::Vector> simplified;
		d2d::DouglasPeucker::implement(itr->first->getVertices(), m_cmpt->getSimplifyThreshold(), simplified);
		itr->second->setVertices(simplified);
	}

	m_editPanel->Refresh();
}

void EditPolylinesOP::updateFromSimplified()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
		itr->first->setVertices(itr->second->getVertices());

	m_editPanel->Refresh();
}

void EditPolylinesOP::clearBuffer()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
		delete itr->second;
	m_simplifyBuffer.clear();

	m_bDirty = false;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::UpdateChainVisitor
//////////////////////////////////////////////////////////////////////////

void EditPolylinesOP::UpdateChainVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	chain->refresh();

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::UpdateBufferVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::UpdateBufferVisitor::
UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer)
	: m_simplifyBuffer(simplifyBuffer)
{
}

void EditPolylinesOP::UpdateBufferVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	m_simplifyBuffer.insert(std::make_pair(chain, chain->clone()));

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::OffsetVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::OffsetVisitor::
OffsetVisitor(const d2d::Vector& offset)
	: m_offset(offset)
{
}

void EditPolylinesOP::OffsetVisitor::
visit(Object* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	std::vector<d2d::Vector> to(chain->getVertices());
	for (size_t i = 0, n = to.size(); i < n; ++i)
		to[i] += m_offset;
	chain->setVertices(to);

	bFetchNext = true;
}

}