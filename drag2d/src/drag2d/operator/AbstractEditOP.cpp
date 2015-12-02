#include "AbstractEditOP.h"

#include "view/EditPanelImpl.h"

namespace d2d
{

AbstractEditOP::AbstractEditOP(wxWindow* wnd, EditPanelImpl* stage)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_cursor(wxCursor(wxCURSOR_ARROW)) 
{
}

bool AbstractEditOP::OnActive() 
{ 
	m_stage->SetCursor(m_cursor);
	return false; 
}

}