#include "LibraryPanel.h"
#include "LibraryList.h"
#include "LibraryShapePage.h"
#include "LibraryImagePage.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <wx/notebook.h>

namespace eshape
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: d2d::LibraryPanel(parent)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
	m_shapePage = new LibraryShapePage(getNotebook());
	addPage(m_shapePage);
	m_imagePage = new LibraryImagePage(getNotebook());
	addPage(m_imagePage);
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	d2d::LibraryPanel::onPageChanged(event);
	
	if (m_stage && m_toolbar)
	{
		m_stage->changeCurrItem(NULL);
		m_toolbar->changeCurrItem(NULL);
	}

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

void LibraryPanel::setContext(StagePanel* stage, ToolbarPanel* toolbar) 
{
	m_stage = stage;
	m_toolbar = toolbar;
	m_shapePage->setContext(stage, toolbar);
	m_imagePage->setContext(stage, toolbar);
}

}