#ifndef _LR_VIEWLIST_IMPL_H_
#define _LR_VIEWLIST_IMPL_H_

#include <drag2d.h>

#include "OpenSymbolDialog.h"

namespace lr
{

class ViewlistListImpl : public d2d::ViewlistListImpl
{
public:
	ViewlistListImpl(wxWindow* wnd, d2d::EditPanelImpl* stage,
		d2d::MultiSpritesImpl* sprites_impl);

	virtual void OnSelected(d2d::ViewlistList* list, int idx);
	virtual void OnDoubleClicked(d2d::ViewlistList* list, int idx);

private:
	OpenSymbolDialog m_open_symbol;

}; // ViewlistListImpl

}

#endif // _LR_VIEWLIST_IMPL_H_