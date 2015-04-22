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

void LibraryPage::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	Json::Value layer_val = value[m_key];
	int idx = 0;
	Json::Value symbol_val = layer_val["symbol"][idx++];
	while (!symbol_val.isNull()) {
		std::string filepath = dir + "//" + symbol_val.asString();
		
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		symbol->RefreshThumbnail(filepath);
		m_list->insert(symbol);
		symbol->Release();

		symbol_val = layer_val["symbol"][idx++];
	}
}

void LibraryPage::StoreToFile(Json::Value& value, const std::string& dir) const
{
	Json::Value& layer_val = value[m_key];
	int idx = 0;
	while (d2d::ISymbol* symbol = m_list->getSymbol(idx)) {
		std::string filepath = d2d::FilenameTools::getRelativePath(dir, symbol->getFilepath());
		layer_val["symbol"][idx] = filepath;
		++idx;
	}
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

}