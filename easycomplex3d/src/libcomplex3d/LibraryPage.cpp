#include "LibraryPage.h"
#include "Symbol.h"
#include "config.h"

#include <ee/LibraryList.h>

#include <node3/ParametricEquations.h>
#include <node3/ParametricSurface.h>
#include <node3/ModelParametric.h>
#include <node3/AssimpHelper.h>
#include <node3/ObjectModel.h>
#include <sprite2/SymType.h>

namespace ecomplex3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "3D")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_MODEL;
}

void LibraryPage::LoadDefaultSymbol()
{
	n3::Surface* surface;

	surface = new n3::Cone(2, 1);
	LoadSymbol(surface, "Cone");

	surface = new n3::Sphere(1);
	LoadSymbol(surface, "Sphere");

	surface = new n3::Torus(0.5f, 0.2f);
	LoadSymbol(surface, "Torus");

	surface = new n3::TrefoilKnot(1);
	LoadSymbol(surface, "TrefoilKnot");

	surface = new n3::MobiusStrip(0.2f);
	LoadSymbol(surface, "MobiusStrip");

	surface = new n3::KleinBottle(0.1f);
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
//  			auto sym = ee::SymbolMgr::Instance()->fetchSymbol(filenames[i]);
//  			m_list->insert(sym);
//  			sym->RemoveReference();
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
			auto model = std::unique_ptr<n3::Model>(
				n3::AssimpHelper::Load(filenames[i].ToStdString()));
			auto obj_model = std::make_shared<n3::ObjectModel>();
			obj_model->SetModel(model);

			auto sym = std::make_shared<Symbol>();
			sym->SetModel(obj_model);

			std::string filepath = FILE_TAG;
			filepath += ".json";
			sym->SetFilepath(filepath);
// 			sym->SetAABB(aabb);

			AddItem(sym);
		}
	}
}

void LibraryPage::LoadSymbol(n3::Surface* surface, const char* name)
{
	n3::AABB aabb;
	auto model = std::unique_ptr<n3::Model>(new n3::ModelParametric(surface, aabb));
	auto obj_model = std::make_shared<n3::ObjectModel>();
	obj_model->SetModel(model);

	auto sym = std::make_shared<Symbol>();
	sym->SetAABB(aabb);
	sym->SetName(name);
	sym->SetModel(obj_model);

	std::string filepath = FILE_TAG;
	filepath += ".json";
	sym->SetFilepath(filepath);

//	sym->RefreshThumbnail(sym->GetFilepath());
	m_list->Insert(sym);
}

}