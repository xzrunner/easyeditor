#include "LayerLibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>
#include <ee/ImageSymbol.h>

namespace emask
{

LayerLibraryPage::LayerLibraryPage(wxWindow* parent, const std::string& name, bool only_img)
	: ee::LibraryPage(parent, name)
	, m_only_img(only_img)
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LayerLibraryPage::IsHandleSymbol(ee::Symbol* symbol) const 
{ 
	if (m_only_img) {
		return dynamic_cast<ee::ImageSymbol*>(symbol) != NULL;
	} else {
		return true; 
	}
}

void LayerLibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string filter;
	if (m_only_img) {
		filter = "*.png;*.jpg;*.bmp;*.pvr";
	} else {
		filter = "*.*";
	}
	wxFileDialog dlg(this, wxT("µº»Îsymbol"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			std::string filepath = filenames[i].ToStdString();
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->Insert(symbol);
			symbol->Release();
		}
	}
}

}