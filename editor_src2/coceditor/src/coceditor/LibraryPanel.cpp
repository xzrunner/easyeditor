#include "LibraryPanel.h"
#include "LibraryComplexPage.h"
#include "Context.h"

#include <wx/notebook.h>

namespace coceditor
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	Context* context = Context::Instance();

	m_imagePage = new d2d::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new LibraryComplexPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new d2d::LibraryAnimPage(getNotebook());
	addPage(m_animPage);
	m_9patchPage = new d2d::Library9PatchPage(getNotebook());
	addPage(m_9patchPage);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
}

//void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
//{
//	d2d::LibraryPanel::onPageChanged(event);
//
//	Context::Instance()->changeCurrItem(NULL);
//
//	if (event.GetSelection() == 0)
//		m_shapePage->clickListSelection();
//	else if (event.GetSelection() == 1)
//		m_imagePage->clickListSelection();
//}

d2d::ILibraryPage* LibraryPanel::getImagePage()
{
	return m_imagePage;
}

d2d::ILibraryPage* LibraryPanel::getComplexPage()
{
	return m_complexPage;
}

d2d::ILibraryPage* LibraryPanel::getAnimPage()
{
	return m_animPage;
}

d2d::ILibraryPage* LibraryPanel::get9PatchPage()
{
	return m_9patchPage;
}
} // coceditor