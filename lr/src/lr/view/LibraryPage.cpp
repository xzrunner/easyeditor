#include "LibraryPage.h"
#include "StagePanel.h"

namespace lr
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name,
						 const std::string& key, StagePanel* stage)
	: d2d::ILibraryPage(parent, name)
	, m_key(key)
	, m_stage(stage)
{
	initLayout();
}

LibraryPage::~LibraryPage()
{
//	m_stage->GetSymbolsCfg().StoreLibraryPage(m_key, this);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("µº»Îsymbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->insert(symbol);
			symbol->Release();
		}

		if (!filenames.empty() && m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

void LibraryPage::StoreToCfgFile()
{
	
}

}