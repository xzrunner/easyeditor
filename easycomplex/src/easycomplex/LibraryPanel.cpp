#include "LibraryPanel.h"

#include <wx/notebook.h>

namespace ecomplex
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	m_imagePage = new ee::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new ee::LibraryComplexPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new ee::LibraryAnimPage(getNotebook());
	addPage(m_animPage);
	m_scale9Page = new ee::Library9PatchPage(getNotebook());
	addPage(m_scale9Page);
	m_fontPage = new ee::LibraryFontBlankPage(getNotebook());
	addPage(m_fontPage);
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{
}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{
}
} // ecomplex