#ifndef _EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_
#define _EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_

#include "OpenSymbolDialog.h"

#include <ee/ViewlistListImpl.h>

namespace ecomplex
{

class ViewlistListImpl : public ee::ViewlistListImpl
{
public:
	ViewlistListImpl(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	virtual void OnSelected(ee::ViewlistList* list, int idx, bool clear) override;
	virtual void OnDoubleClicked(ee::ViewlistList* list, int idx) override;

private:
	OpenSymbolDialog m_open_symbol;

}; // ViewlistListImpl

}

#endif // _EASYCOMPLEX_VIEWLIST_LIST_IMPL_H_