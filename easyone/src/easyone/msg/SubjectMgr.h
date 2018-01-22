#pragma once

#include "msg/MessageID.h"
#include "msg/Subject.h"

#include <map>
#include <memory>

namespace eone
{

class SubjectMgr
{
public:
	Subject& AddSubject(MessageID id);
	Subject* QuerySubject(MessageID id) const;

	bool RegisterObserver(MessageID msg, Observer* o);
	bool UnregisterObserver(MessageID msg, Observer* o);

private:
	std::map<MessageID, std::unique_ptr<Subject>> m_subjects;

}; // SubjectMgr

}