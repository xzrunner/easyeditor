#ifndef _LR_PREVIEW_PATH_UTIL_H_
#define _LR_PREVIEW_PATH_UTIL_H_

#include <SM_Vector.h>

#include <functional>
#include <set>

namespace lr
{
namespace preview
{

class INetwork
{
public:
	virtual sm::vec2 TransIDToPos(int id) const = 0;

}; // INetwork

class VisitedNode
{
public:
	VisitedNode(int id, VisitedNode* prev = NULL, float from = 0, float to = 0)
		: m_id(id), m_prev(prev), m_from(from), m_to(to) {}

	struct LengthAscending : public std::binary_function<VisitedNode*, VisitedNode*, bool>
	{
		bool operator() (const VisitedNode* lhs, const VisitedNode* rhs) const {
			return lhs->m_from + lhs->m_to < rhs->m_from + rhs->m_to
				|| lhs->m_from + lhs->m_to == rhs->m_from + rhs->m_to && lhs->m_id < rhs->m_id;
		}
	};

	struct IDAscending : public std::binary_function<VisitedNode*, VisitedNode*, bool>
	{
		bool operator() (const VisitedNode* lhs, const VisitedNode* rhs) const {
			return lhs->m_id < rhs->m_id;
		}
	};

public:
	int m_id;

	float m_from, m_to;

	VisitedNode* m_prev;

}; // VisitedNode

class VisitedList
{
public:
	VisitedList(INetwork* nw) : m_nw(nw) {}

	void Push(VisitedNode* n) { m_data.insert(n); }

	VisitedNode* Find(int id);

	void Clear();

	void DebugDraw() const;

private:
	INetwork* m_nw;

	std::set<VisitedNode*, VisitedNode::IDAscending> m_data;

}; // VisitedList

class CandidateList
{
public:
	bool IsEmpty() const { return m_data.empty(); }

	void Push(VisitedNode* n) { m_data.insert(n); }

	VisitedNode* Top() const { return *m_data.begin(); }

	void Pop() { m_data.erase(m_data.begin()); }

	void Update(VisitedNode* v, float len, VisitedNode* src);

	void Clear() { m_data.clear(); }

private:
	std::set<VisitedNode*, VisitedNode::LengthAscending> m_data;	

}; // CandidateList

}
}

#endif // _LR_PREVIEW_PATH_UTIL_H_