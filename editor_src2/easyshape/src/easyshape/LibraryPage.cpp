
#include "LibraryPage.h"
#include "LibraryList.h"

#include "Context.h"

using namespace eshape;

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: d2d::ILibraryPage(parent, name)
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

void LibraryPage::onDelPress(wxCommandEvent& event)
{
	d2d::ILibraryPage::onDelPress(event);
	Context::Instance()->changeCurrItem(NULL);
}