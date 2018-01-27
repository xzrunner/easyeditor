#include "ee0/SubjectMgr.h"
#include "ee0/Subject.h"

#include <guard/check.h>

namespace ee0
{

void SubjectMgr::RegisterObserver(MessageID msg, Observer* o)
{
	auto sub = QuerySubject(msg);
	if (!sub) {
		auto sub_ptr = std::make_unique<Subject>(msg);
		sub = sub_ptr.get();
		m_subjects.insert(std::make_pair(msg, std::move(sub_ptr)));
	} 

	sub->RegisterObserver(o);
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

bool SubjectMgr::NotifyObservers(MessageID msg, const VariantSet& variants)
{
	auto sub = QuerySubject(msg);
	if (!sub) {
		return false;
	} else {
		sub->NotifyObservers(variants);
		return true;
	}
}

Subject* SubjectMgr::QuerySubject(MessageID id) const
{
	auto& itr = m_subjects.find(id);
	return itr == m_subjects.end() ? nullptr : itr->second.get();
}

}