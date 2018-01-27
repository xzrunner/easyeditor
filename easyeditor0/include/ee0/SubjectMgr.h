#pragma once

#include "ee0/MessageID.h"
#include "ee0/Subject.h"
#include "ee0/VariantSet.h"

#include <map>
#include <memory>

namespace ee0
{

class SubjectMgr
{
public:
	void RegisterObserver(MessageID msg, Observer* o);
	bool UnregisterObserver(MessageID msg, Observer* o);

	bool NotifyObservers(MessageID msg, 
		const VariantSet& variants = VariantSet());

private:
	Subject* QuerySubject(MessageID id) const;

private:
	std::map<MessageID, std::unique_ptr<Subject>> m_subjects;

}; // SubjectMgr

}