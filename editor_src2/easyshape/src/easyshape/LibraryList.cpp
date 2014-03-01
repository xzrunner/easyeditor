
#include "LibraryList.h"
#include "LibraryItem.h"
#include "Context.h"

using namespace eshape;

LibraryList::LibraryList(wxWindow* parent)
	: d2d::LibraryList(parent)
{
}

void LibraryList::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryList::storeToTextFile(std::ofstream& fout) const
{

}

void LibraryList::onListSelected(wxCommandEvent& event)
{
	if (m_items.empty()) return;

	LibraryItem* item = static_cast<LibraryItem*>(m_items[event.GetInt()]);
	Context::Instance()->changeCurrItem(item);
}