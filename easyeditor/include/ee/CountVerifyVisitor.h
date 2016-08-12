#ifndef _EASYEDITOR_COUNT_VERIFY_VISITOR_H_
#define _EASYEDITOR_COUNT_VERIFY_VISITOR_H_

#include "Visitor.h"
#include "Shape.h"

namespace ee
{

class CountVerifyVisitor : public Visitor<Shape>
{
public:
	CountVerifyVisitor(bool& valid, int num)
		: m_valid(valid), m_num(num), m_count(0) {}

	virtual ~CountVerifyVisitor() {
		m_valid = m_count == m_num;
	}

	void Visit(Shape* shape, bool& next) {
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