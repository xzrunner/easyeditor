#ifndef _EASYEDITOR_VIEW_LIST_IMPL_H_
#define _EASYEDITOR_VIEW_LIST_IMPL_H_

namespace ee
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

#endif // _EASYEDITOR_VIEW_LIST_IMPL_H_