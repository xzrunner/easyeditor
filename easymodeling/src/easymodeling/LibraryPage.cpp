
#include "LibraryPage.h"
#include "LibraryList.h"

using namespace emodeling;

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::LibraryImagePage(parent)
{
	m_list = new LibraryList(this);
	InitLayout();
}