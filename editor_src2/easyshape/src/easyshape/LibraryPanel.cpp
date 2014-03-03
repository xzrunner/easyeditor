
#include "LibraryPanel.h"
#include "LibraryList.h"
#include "LibraryShapePage.h"
#include "LibraryImagePage.h"
#include "Context.h"

#include <wx/notebook.h>

using namespace eshape;

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	Context* context = Context::Instance();

	m_shapePage = new LibraryShapePage(getNotebook());
	addPage(m_shapePage);
	m_imagePage = new LibraryImagePage(getNotebook());
	addPage(m_imagePage);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
	m_imagePage->loadFromTextFile(fin);
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
	m_imagePage->storeToTextFile(fout);
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::onPageChanged(event);
	
	Context::Instance()->changeCurrItem(NULL);

	if (event.GetSelection() == 0)
		m_shapePage->clickListSelection();
	else if (event.GetSelection() == 1)
		m_imagePage->clickListSelection();
}

d2d::ILibraryPage* LibraryPanel::getShapePage() 
{ 
	return m_shapePage; 
}

d2d::ILibraryPage* LibraryPanel::getImagePage() 
{ 
	return m_imagePage; 
}