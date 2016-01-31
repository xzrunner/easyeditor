#include "EditOP.h"
#include "EditPanelImpl.h"

namespace ee
{

EditOP::EditOP(wxWindow* wnd, EditPanelImpl* stage)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_cursor(wxCursor(wxCURSOR_ARROW)) 
{
}

bool EditOP::OnActive() 
{ 
	m_stage->SetCursor(m_cursor);
	return false; 
}

}