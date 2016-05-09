#include "EditPolylinesOP.h"
#include "EditPolylinesCMPT.h"
#include "ChainShape.h"

#include <ee/EditPanelImpl.h>
#include <ee/ShapeSelection.h>
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
	, m_is_dirty(false)
	, m_last_pos_valid(false)
{
	clearBuffer();
}

bool EditPolylinesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftDown(x, y)) return true;

	if (!m_last_pos_valid) {
		m_last_pos = m_stage->TransPosScrToProj(x, y);
		m_last_pos_valid = true;
	} else {
		m_last_pos_valid = false;
	}

	return false;
}

bool EditPolylinesOP::OnMouseLeftUp(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseLeftUp(x, y)) return true;

	if (m_is_dirty)
		m_selection->Traverse(UpdateChainVisitor());

	clearBuffer();
	m_selection->Traverse(UpdateBufferVisitor(m_simplify_buffer));

	return false;
}

bool EditPolylinesOP::OnMouseDrag(int x, int y)
{
	if (ee::SelectShapesOP::OnMouseDrag(x, y)) return true;

	if (m_last_pos_valid)
	{
		sm::vec2 curr_pos = m_stage->TransPosScrToProj(x, y);
		sm::vec2 offset = curr_pos - m_last_pos;
		m_selection->Traverse(OffsetVisitor(offset));
		m_last_pos = curr_pos;
		m_last_pos_valid = true;

		m_is_dirty = true;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool EditPolylinesOP::OnDraw() const
{
	if (ee::SelectShapesOP::OnDraw()) return true;

	ee::RenderColor color;
	color.multi.Set(0.8f, 0.8f, 0.2f);

	std::map<ChainShape*, ChainShape*>::const_iterator itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr) {
		itr->second->Draw(sm::mat4(), color);
		ee::RVG::Color(ee::Colorf(0.2f, 0.2f, 0.8f));
		ee::RVG::Circles(itr->second->GetVertices(), ee::SettingData::ctl_pos_sz, true);
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
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr)
	{
		std::vector<sm::vec2> simplified;
		ee::DouglasPeucker::Do(itr->first->GetVertices(), m_cmpt->GetSimplifyThreshold(), simplified);
		itr->second->Load(simplified);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPolylinesOP::updateFromSimplified()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr)
		itr->first->Load(itr->second->GetVertices());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void EditPolylinesOP::clearBuffer()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplify_buffer.begin();
	for ( ; itr != m_simplify_buffer.end(); ++itr)
		delete itr->second;
	m_simplify_buffer.clear();

	m_is_dirty = false;
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
OffsetVisitor(const sm::vec2& offset)
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