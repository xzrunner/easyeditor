#include "ViewlistListImpl.h"

#include <ee/ViewlistList.h>

namespace ecomplex
{

ViewlistListImpl::ViewlistListImpl(wxWindow* wnd, ee::EditPanelImpl* stage, 
								   ee::MultiSpritesImpl* sprites_impl)
	: m_open_symbol(wnd, stage, sprites_impl)
{
}

void ViewlistListImpl::OnSelected(ee::ViewlistList* list, int idx, bool clear)
{
	list->OnSelected(idx, clear);
}

void ViewlistListImpl::OnDoubleClicked(ee::ViewlistList* list, int idx)
{
	ee::Sprite* selected = list->QuerySprite(idx);
	m_open_symbol.Open(selected);
}

}