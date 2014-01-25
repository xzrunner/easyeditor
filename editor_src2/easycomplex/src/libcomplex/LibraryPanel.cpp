#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace libcomplex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	m_imagePage = new d2d::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new d2d::LibraryComplexPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new d2d::LibraryAnimPage(getNotebook());
	addPage(m_animPage);
	m_9patchPage = new d2d::Library9PatchPage(getNotebook());
	addPage(m_9patchPage);
	m_fontPage = new d2d::LibraryFontBlankPage(getNotebook());
	addPage(m_fontPage);
	m_scriptsPage = new d2d::LibraryScriptsPage(getNotebook());
	addPage(m_scriptsPage);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
}
} // ecomplex