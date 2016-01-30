#ifndef _EASYEDITOR_GET_NAME_VISITOR_H_
#define _EASYEDITOR_GET_NAME_VISITOR_H_

#include "Visitor.h"

namespace ee
{

template <typename T>
class GetNameVisitor : public Visitor
{
public:
	GetNameVisitor(std::vector<wxString>& names)
		: m_names(names) {}

	virtual void Visit(Object* object, bool& next)
	{
		T* item = dynamic_cast<T*>(object);
		if (item)
			m_names.push_back(item->getName());
		next = true;
	}

private:
	std::vector<wxString>& m_names;

}; // GetNameVisitor

}

#endif // _EASYEDITOR_GET_NAME_VISITOR_H_