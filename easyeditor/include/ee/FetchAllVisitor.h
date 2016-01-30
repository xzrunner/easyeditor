#ifndef _EASYEDITOR_FETCH_ALL_VISITOR_H_
#define _EASYEDITOR_FETCH_ALL_VISITOR_H_

#include "Visitor.h"

namespace ee
{

template <typename T>
class FetchAllVisitor : public Visitor
{
public:
	FetchAllVisitor(std::vector<T*>& result) 
		: m_result(result) {}

	virtual void Visit(Object* object, bool& next)
	{
		T* item = dynamic_cast<T*>(object);
		if (item)
			m_result.push_back(item);
		next = true;
	}

private:
	std::vector<T*>& m_result;

}; // FetchAllVisitor

}

#endif // _EASYEDITOR_FETCH_ALL_VISITOR_H_