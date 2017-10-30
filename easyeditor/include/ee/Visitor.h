#ifndef _EASYEDITOR_VISITOR_H_
#define _EASYEDITOR_VISITOR_H_

#include <memory>

namespace ee
{

template<class T>
class Visitor
{
public:
	virtual void Visit(T* obj, bool& next) = 0;
	virtual ~Visitor() {}
}; // Visitor

template<class T>
class RefVisitor
{
public:
	virtual void Visit(const std::shared_ptr<T>& obj, bool& next) = 0;
	virtual ~RefVisitor() {}
}; // RefVisitor

}

#endif // _EASYEDITOR_VISITOR_H_
