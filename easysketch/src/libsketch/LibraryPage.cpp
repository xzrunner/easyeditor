#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace libsketch
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("3D"))
{
	initLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

// void LibraryPage::onAddPress(wxCommandEvent& event)
// {
// 	wxString filter = wxString(wxT("*_")) + FILE_TAG + wxT(".json");
// 	wxFileDialog dlg(this, wxT("choose obj files"), wxEmptyString, 
// 		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
// 	if (dlg.ShowModal() == wxID_OK)
// 	{
// 		wxArrayString filenames;
// 		dlg.GetPaths(filenames);
// 		for (size_t i = 0, n = filenames.size(); i < n; ++i)
// 		{
//  			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filenames[i]);
//  			m_list->insert(symbol);
//  			symbol->Release();
// 		}
// 	}
// }

// for test assimp loader
void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("choose model files"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			e3d::ModelParametric* model = new e3d::ModelParametric();
			e3d::AssimpHelper loader;
			e3d::AABB aabb;
			loader.LoadFile(filenames[i], *model, aabb);
			libsketch::Symbol* symbol = new libsketch::Symbol();
			symbol->SetModel(model);

			wxString filepath = FILE_TAG;
			filepath += ".json";
			symbol->SetFilepath(filepath);
			symbol->SetAABB(aabb);

			AddItem(symbol);
		}
	}
}

}