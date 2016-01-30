#ifndef _EASYEDITOR_VISITOR_H_
#define _EASYEDITOR_VISITOR_H_

namespace ee
{

class Object;

class Visitor
{
public:
	virtual void Visit(Object* object, bool& next) = 0;
	virtual ~Visitor() {}
}; // Visitor

}

#endif // _EASYEDITOR_VISITOR_H_
