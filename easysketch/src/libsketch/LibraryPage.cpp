#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

namespace libsketch
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("3D"))
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::LoadDefaultSymbol()
{
	e3d::ISurface* surface;

	surface = new e3d::Cone(2, 1);
	LoadSymbol(surface, "Cone");

	surface = new e3d::Sphere(1);
	LoadSymbol(surface, "Sphere");

	surface = new e3d::Torus(0.5f, 0.2f);
	LoadSymbol(surface, "Torus");

	surface = new e3d::TrefoilKnot(1);
	LoadSymbol(surface, "TrefoilKnot");

	surface = new e3d::MobiusStrip(0.2f);
	LoadSymbol(surface, "MobiusStrip");

	surface = new e3d::KleinBottle(0.1f);
	LoadSymbol(surface, "KleinBottle");
}

// void LibraryPage::OnAddPress(wxCommandEvent& event)
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
void LibraryPage::OnAddPress(wxCommandEvent& event)
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

			std::string filepath = FILE_TAG;
			filepath += ".json";
			symbol->SetFilepath(filepath);
			symbol->SetAABB(aabb);

			AddItem(symbol);
		}
	}
}

void LibraryPage::LoadSymbol(e3d::ISurface* surface, const char* name)
{
	e3d::AABB aabb;
	e3d::ModelParametric* model = new e3d::ModelParametric(surface, aabb);

	libsketch::Symbol* symbol = new libsketch::Symbol();
	symbol->SetAABB(aabb);
	symbol->SetName(name);
	symbol->SetModel(model);

	std::string filepath = FILE_TAG;
	filepath += ".json";
	symbol->SetFilepath(filepath);

//	symbol->RefreshThumbnail(symbol->GetFilepath());
	m_list->Insert(symbol);
}

}