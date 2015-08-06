#include "AbstractEditOP.h"

#include "view/EditPanel.h"
#include "view/EditPanelImpl.h"

namespace d2d
{

AbstractEditOP::AbstractEditOP(wxWindow* wnd, EditPanelImpl* stage)
	: m_wnd(wnd)
	, m_cursor(wxCursor(wxCURSOR_ARROW)) 
{
	if (stage) {
		stage->Retain();
	}
	m_stage = stage;
}

AbstractEditOP::~AbstractEditOP()
{
	if (m_stage) {
		m_stage->Release();
	}
}

bool AbstractEditOP::OnActive() 
{ 
	m_stage->SetCursor(m_cursor);
	return false; 
}

}