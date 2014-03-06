#include "LibraryPanel.h"

#include <wx/notebook.h>

#include <easyanim.h>
#include <easycomplex.h>

namespace eparticle3d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
{
	m_imagePage = new d2d::LibraryImagePage(getNotebook());
	addPage(m_imagePage);
	m_complexPage = new ecomplex::LibraryPage(getNotebook());
	addPage(m_complexPage);
	m_animPage = new anim::LibraryPage(getNotebook());
	addPage(m_animPage);
	m_scale9Page = new d2d::LibraryScale9Page(getNotebook());
	addPage(m_scale9Page);
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

}