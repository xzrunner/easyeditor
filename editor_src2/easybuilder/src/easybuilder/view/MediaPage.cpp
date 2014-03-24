
#include "MediaPage.h"
#include "ListObserver.h"
#include "Context.h"
#include "StagePanel.h"

#include "output/DesignerPage.h"

using namespace ebuilder;

MediaPage::MediaPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Media"))
{
	initLayout();

	m_observer = new ListObserver(m_list);
}

MediaPage::~MediaPage()
{
	delete m_observer;
}

bool MediaPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<d2d::ImageSymbol*>(symbol) != NULL 
		|| dynamic_cast<d2d::FontSymbol*>(symbol) != NULL;
}

void MediaPage::onAddPress(wxCommandEvent& event)
{
	// todo: after add a new media int MediaPage's "+", selected will be dragged
	Context::Instance()->stage->getDesignerPage()->getSpriteSelection()->clear();

	wxString formatFilter = wxT("*.png;*.jpg;*_combination.txt;*.ttf");
	wxFileDialog dlg(this, wxT("Choose symbol files"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
		}

		m_observer->updateListeners();
	}
}

void MediaPage::onDelPress(wxCommandEvent& event)
{
	d2d::ILibraryPage::onDelPress(event);
	m_observer->updateListeners();
}
