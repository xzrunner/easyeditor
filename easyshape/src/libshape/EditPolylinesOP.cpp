#include "EditPolylinesOP.h"
#include "EditPolylinesCMPT.h"
#include "ChainShape.h"

#include <ee/EditPanelImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/Matrix.h>
#include <ee/EE_RVG.h>
#include <ee/panel_msg.h>
#include <ee/SettingData.h>
#include <ee/DouglasPeucker.h>

namespace eshape
{

EditPolylinesOP::EditPolylinesOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
								 ee::MultiShapesImpl* shapes_impl,
								 EditPolylinesCMPT* cmpt)
	: ee::SelectShapesOP(wnd, stage, shapes_impl, cmpt)
	, m_cmpt(cmpt)
	, m_bDirty(false)
{
	m_lastPos.SetInvalid();

	clearBuffer();
}

bool EditPolylinesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftDown(x, y)) return true;

	if (!m_first_pos.IsValid())
		m_lastPos = m_stage->TransPosScrToProj(x, y);
	else
		m_lastPos.SetInvalid();

	return false;
}

bool EditPolylinesOP::OnMouseLeftUp(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftUp(x, y)) return true;

	if (m_bDirty)
		m_selection->Traverse(UpdateChainVisitor());

	clearBuffer();
	m_selection->Traverse(UpdateBufferVisitor(m_simplifyBuffer));

	return false;
}

bool EditPolylinesOP::OnMouseDrag(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseDrag(x, y)) return true;

	if (m_lastPos.IsValid())
	{
		ee::Vector currPos = m_stage->TransPosScrToProj(x, y);
		ee::Vector offset = currPos - m_lastPos;
		m_selection->Traverse(OffsetVisitor(offset));
		m_lastPos = currPos;

		m_bDirty = true;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditPolylinesOP::OnDraw() const
{
	if (ee::SelectShapesOP::OnDraw()) return true;

	ee::ColorTrans color;
	color.multi.Set(0.8f, 0.8f, 0.2f);

	std::map<ChainShape*, ChainShape*>::const_iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr) {
		itr->second->Draw(ee::Matrix(), color);
		ee::RVG::Circles(itr->second->GetVertices(), ee::SettingData::ctl_pos_sz, true, ee::Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditPolylinesOP::Clear()
{
	if (ee::SelectShapesOP::Clear()) return true;

	clearBuffer();

	return false;
}

void EditPolylinesOP::simplify()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
	{
		std::vector<ee::Vector> simplified;
		ee::DouglasPeucker::Do(itr->first->GetVertices(), m_cmpt->GetSimplifyThreshold(), simplified);
		itr->second->Load(simplified);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPolylinesOP::updateFromSimplified()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
		itr->first->Load(itr->second->GetVertices());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
Visit(Object* object, bool& next)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	chain->refresh();

	next = true;
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
Visit(Object* object, bool& next)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	m_simplifyBuffer.insert(std::make_pair(chain, chain->Clone()));

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::OffsetVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::OffsetVisitor::
OffsetVisitor(const ee::Vector& offset)
	: m_offset(offset)
{
}

void EditPolylinesOP::OffsetVisitor::
Visit(Object* object, bool& next)
{
	ChainShape* chain = static_cast<ChainShape*>(object);
	chain->Translate(m_offset);
	next = true;
}

}