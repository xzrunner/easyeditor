#ifndef _LR_VIEWLIST_IMPL_H_
#define _LR_VIEWLIST_IMPL_H_

#include "OpenSymbolDialog.h"

#include <ee/ViewlistListImpl.h>

namespace lr
{

class ViewlistListImpl : public ee::ViewlistListImpl
{
public:
	ViewlistListImpl(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	virtual void OnSelected(ee::ViewlistList* list, int idx, bool clear);
	virtual void OnDoubleClicked(ee::ViewlistList* list, int idx);

private:
	OpenSymbolDialog m_open_symbol;

}; // ViewlistListImpl

}

#endif // _LR_VIEWLIST_IMPL_H_