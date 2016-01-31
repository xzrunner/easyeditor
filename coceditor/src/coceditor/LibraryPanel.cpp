#include "LibraryPanel.h"
#include "LibraryComplexPage.h"
#include "Context.h"

#include <wx/notebook.h>

namespace coceditor
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	Context* context = Context::Instance();

	m_imagePage = new ee::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new LibraryComplexPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new ee::LibraryAnimPage(getNotebook());
	addPage(m_animPage);
	m_scale9Page = new ee::Library9PatchPage(getNotebook());
	addPage(m_scale9Page);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
}

//void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
//{
//	ee::LibraryPanel::onPageChanged(event);
//
//	Context::Instance()->changeCurrItem(NULL);
//
//	if (event.GetSelection() == 0)
//		m_shapePage->clickListSelection();
//	else if (event.GetSelection() == 1)
//		m_imagePage->clickListSelection();
//}

ee::ILibraryPage* LibraryPanel::getImagePage()
{
	return m_imagePage;
}

ee::ILibraryPage* LibraryPanel::getComplexPage()
{
	return m_complexPage;
}

ee::ILibraryPage* LibraryPanel::getAnimPage()
{
	return m_animPage;
}

ee::ILibraryPage* LibraryPanel::get9PatchPage()
{
	return m_scale9Page;
}
} // coceditor