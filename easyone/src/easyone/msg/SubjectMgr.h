#pragma once

#include "msg/MessageID.h"
#include "msg/Subject.h"
#include "msg/VariantSet.h"

#include <map>
#include <memory>

namespace eone
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