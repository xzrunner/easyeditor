#ifndef _EASYEDITOR_VISITOR_H_
#define _EASYEDITOR_VISITOR_H_

namespace ee
{

template<class T>
class Visitor
{
public:
	virtual void Visit(T* obj, bool& next) = 0;
	virtual ~Visitor() {}
}; // Visitor

}

#endif // _EASYEDITOR_VISITOR_H_
