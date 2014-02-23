#include "LibraryPanel.h"
#include "LibraryPage.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easymesh.h>

namespace complex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	m_imagePage = new d2d::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new LibraryPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new anim::LibraryPage(getNotebook());
	addPage(m_animPage);
	m_9patchPage = new d2d::Library9PatchPage(getNotebook());
	addPage(m_9patchPage);
	m_fontPage = new d2d::LibraryFontBlankPage(getNotebook());
	addPage(m_fontPage);
	m_meshPage = new emesh::LibraryPage(getNotebook());
	addPage(m_meshPage);
	m_scriptsPage = new d2d::LibraryScriptsPage(getNotebook());
	addPage(m_scriptsPage);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
}

d2d::ILibraryPage* LibraryPanel::getComplexPage() 
{ 
	return m_complexPage; 
}

d2d::ILibraryPage* LibraryPanel::getAnimPage() 
{ 
	return m_animPage; 
}

d2d::ILibraryPage* LibraryPanel::getMeshPage()
{
	return m_meshPage;
}

} // ecomplex