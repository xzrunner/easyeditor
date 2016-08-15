#ifndef _EASYEDITOR_GET_NAME_VISITOR_H_
#define _EASYEDITOR_GET_NAME_VISITOR_H_

#include "Visitor.h"

namespace ee
{

template <typename T>
class GetNameVisitor : public Visitor<T>
{
public:
	GetNameVisitor(std::vector<std::string>& names)
		: m_names(names) {}

	virtual void Visit(T* obj, bool& next)
	{
		if (obj)
			m_names.push_back(obj->getName());
		next = true;
	}

private:
	std::vector<std::string>& m_names;

}; // GetNameVisitor

}

#endif // _EASYEDITOR_GET_NAME_VISITOR_H_