#include "AbstractEditOP.h"

#include "view/EditPanel.h"

namespace d2d
{

AbstractEditOP::AbstractEditOP(EditPanel* editPanel)
	: m_stage(editPanel)
	, m_cursor(wxCursor(wxCURSOR_ARROW)) 
{}

bool AbstractEditOP::OnActive() 
{ 
	m_stage->SetCursor(m_cursor);
	return false; 
}

}