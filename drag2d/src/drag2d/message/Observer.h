#ifndef _DRAG2D_OBSERVER_H_
#define _DRAG2D_OBSERVER_H_

namespace d2d
{

class Subject;

class Observer
{
public:
	virtual ~Observer() {}
	virtual void Notify(int sj_id, void* ud) = 0;

}; // Observer

}

#endif // _DRAG2D_OBSERVER_H_