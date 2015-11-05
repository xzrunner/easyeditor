#ifndef _DRAG2D_VIEW_LIST_IMPL_H_
#define _DRAG2D_VIEW_LIST_IMPL_H_

namespace d2d
{

class ViewlistList;

class ViewlistListImpl
{
public:
	virtual ~ViewlistListImpl() {}
	virtual void OnSelected(ViewlistList* list, int idx) = 0;
	virtual void OnDoubleClicked(ViewlistList* list, int idx) = 0;

}; // ViewlistListImpl

}

#endif // _DRAG2D_VIEW_LIST_IMPL_H_