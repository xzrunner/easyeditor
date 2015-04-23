#include "PathUtil.h"

namespace lr
{
namespace preview
{

//////////////////////////////////////////////////////////////////////////
// class VisitedList
//////////////////////////////////////////////////////////////////////////

VisitedNode* VisitedList::Find(int id) 
{
	VisitedNode n(id);
	std::set<VisitedNode*, VisitedNode::IDAscending>::iterator itr
		= m_data.find(&n);
	if (itr != m_data.end()) {
		return *itr;
	} else {
		return NULL;
	}
}

void VisitedList::Clear() 
{
	std::set<VisitedNode*, VisitedNode::IDAscending>::iterator itr
		= m_data.begin();
	for ( ; itr != m_data.end(); ++itr) {
		delete *itr;
	}
	m_data.clear();
}

void VisitedList::DebugDraw() const
{
	std::set<VisitedNode*, VisitedNode::IDAscending>::const_iterator itr
		= m_data.begin();
	for ( ; itr != m_data.end(); ++itr) {
		d2d::Vector pos = m_nw->TransIDToPos((*itr)->m_id);
		d2d::PrimitiveDraw::drawPoint(pos, d2d::MID_RED, 5);
	}
}

//////////////////////////////////////////////////////////////////////////
// class CandidateList
//////////////////////////////////////////////////////////////////////////

void CandidateList::Update(VisitedNode* v, float len, VisitedNode* src) 
{
	std::set<VisitedNode*, VisitedNode::LengthAscending>::iterator itr
		= m_data.find(v);

	if (itr != m_data.end())
		m_data.erase(itr);

	v->m_from = len;
	v->m_prev = src;
	m_data.insert(v);
}

}
}