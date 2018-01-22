#include "msg/SubjectMgr.h"
#include "msg/Subject.h"

#include <guard/check.h>

namespace eone
{

Subject& SubjectMgr::AddSubject(MessageID id)
{
	GD_ASSERT(QuerySubject(id) == nullptr, "subject exists.");
	auto sub_ptr = std::make_unique<Subject>(id);
	m_subjects.insert(std::make_pair(id, std::move(sub_ptr)));
	return *sub_ptr;
}

Subject* SubjectMgr::QuerySubject(MessageID id) const
{
	auto& itr = m_subjects.find(id);
	return itr == m_subjects.end() ? nullptr : itr->second.get();
}

bool SubjectMgr::RegisterObserver(MessageID msg, Observer* o)
{
	auto sub = QuerySubject(msg);
	if (!sub) {
		return false;
	} else {
		sub->RegisterObserver(o);
		return true;
	}
}

bool SubjectMgr::UnregisterObserver(MessageID msg, Observer* o)
{
	auto sub = QuerySubject(msg);
	if (!sub) {
		return false;
	} else {
		sub->UnregisterObserver(o);
		return true;
	}
}

}