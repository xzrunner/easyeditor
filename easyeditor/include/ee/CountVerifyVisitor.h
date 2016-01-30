#ifndef _EASYEDITOR_COUNT_VERIFY_VISITOR_H_
#define _EASYEDITOR_COUNT_VERIFY_VISITOR_H_

#include "Visitor.h"

namespace ee
{

//	template <typename T>
class CountVerifyVisitor : public Visitor
{
public:
	CountVerifyVisitor(bool& valid, int num)
		: m_valid(valid), m_num(num), m_count(0) {}

	virtual ~CountVerifyVisitor() {
		m_valid = m_count == m_num;
	}

	void Visit(Object* object, bool& next) {
		++m_count;
		next = m_count <= m_num;
	}

private:
	bool& m_valid;
	const int m_num;

	int m_count;

}; // CountVerifyVisitor

}

#endif // _EASYEDITOR_COUNT_VERIFY_VISITOR_H_