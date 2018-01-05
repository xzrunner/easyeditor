#include "LibraryPage.h"
#include "ModelFile.h"
#include "Symbol.h"

#include <ee/LibraryList.h>

#include <sprite2/ModelSymbol.h>
#include <sprite2/SymType.h>
#include <node3/ModelType.h>
#include <node3/AssimpHelper.h>
#include <node3/Model.h>
#include <node3/ObjectModel.h>

namespace e3d
{

LibraryPage::LibraryPage(wxWindow* parent, const std::string& title)
	: ee::LibraryPage(parent, title)
{
	InitLayout();
//	m_list->SetFileter(ModelFile::Instance()->Tag(n3::MODEL_OBJECT));
	m_list->SetFileter(title);
}

bool LibraryPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_MODEL;
}

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

			std::string filepath = ModelFile::Instance()->Tag(n3::MODEL_OBJECT);
			filepath += ".json";
			//sym->SetFilepath(filepath);
// 			sym->SetAABB(aabb);

			AddItem(sym);
		}
	}
}

}