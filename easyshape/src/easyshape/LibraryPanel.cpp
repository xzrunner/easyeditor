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
	: ee::LibraryPanel(parent)
	, m_stage(NULL)
	, m_toolbar(NULL)
{
	wxWindow* nb = getNotebook();
	m_shapePage = new LibraryShapePage(nb);
	addPage(m_shapePage);
	m_imagePage = new LibraryImagePage(nb);
	addPage(m_imagePage);
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	ee::LibraryPanel::onPageChanged(event);
	
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

ee::ILibraryPage* LibraryPanel::getShapePage() 
{ 
	return m_shapePage; 
}

ee::ILibraryPage* LibraryPanel::getImagePage() 
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