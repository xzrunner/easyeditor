
#include "LibraryList.h"
#include "LibraryItem.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

using namespace eshape;

LibraryList::LibraryList(wxWindow* parent)
	: d2d::LibraryList(parent)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
}

void LibraryList::onListSelected(wxCommandEvent& event)
{
	if (m_items.empty()) return;

	if (m_stage && m_toolbar) 
	{
		LibraryItem* item = static_cast<LibraryItem*>(m_items[event.GetInt()]);
		m_stage->changeCurrItem(item);
		m_toolbar->changeCurrItem(item);
	}
}