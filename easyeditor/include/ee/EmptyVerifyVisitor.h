#ifndef _EASYEDITOR_EMPTY_VERIFY_VISITOR_H_
#define _EASYEDITOR_EMPTY_VERIFY_VISITOR_H_

#include "Visitor.h"

namespace ee
{

class EmptyVerifyVisitor : public Visitor
{
public:
	EmptyVerifyVisitor(bool& valid) 
		: m_valid(valid) { m_valid = true; }

	void Visit(Object* object, bool& next) {
		m_valid = false;
		next = false;
	}

private:
	bool& m_valid;

}; // EmptyVerifyVisitor

}

#endif // _EASYEDITOR_EMPTY_VERIFY_VISITOR_H_