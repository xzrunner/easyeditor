
#include "LibraryPage.h"
#include "LibraryList.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

using namespace eshape;

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: d2d::ILibraryPage(parent, name)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
	m_list = new LibraryList(this);
}

void LibraryPage::clickListSelection()
{
	int index = m_list->GetSelection();
	if (index != -1)
	{
		wxCommandEvent event;
		event.SetInt(index);
		m_list->onListSelected(event);
	}
}

void LibraryPage::setContext(StagePanel* stage, ToolbarPanel* toolbar) 
{
	m_stage = stage;
	m_toolbar = toolbar;

	LibraryList* list = static_cast<LibraryList*>(m_list);
	list->setContext(stage, toolbar);
}

void LibraryPage::onDelPress(wxCommandEvent& event)
{
	d2d::ILibraryPage::onDelPress(event);

	if (m_stage && m_toolbar)
	{
		m_stage->changeCurrItem(NULL);
		m_toolbar->changeCurrItem(NULL);
	}
}