#ifndef _EASY_EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_
#define _EASY_EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_

#include "OpenSymbolDialog.h"

#include <ee/ViewlistListImpl.h>

namespace ecomplex
{

class ViewlistListImpl : public ee::ViewlistListImpl
{
public:
	ViewlistListImpl(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	virtual void OnSelected(ee::ViewlistList* list, int idx);
	virtual void OnDoubleClicked(ee::ViewlistList* list, int idx);

private:
	OpenSymbolDialog m_open_symbol;

}; // ViewlistListImpl

}

#endif // _EASY_EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_