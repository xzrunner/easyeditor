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

namespace enode3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Model")
{
	InitLayout();
//	m_list->SetFileter(ModelFile::Instance()->Tag(n3::MODEL_OBJECT));
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
			n3::AABB aabb;
			auto model = n3::AssimpHelper::Load(filenames[i].ToStdString(), aabb);
			if (!model) {
				continue;
			}

			auto obj_model = std::make_shared<n3::ObjectModel>();
			obj_model->SetModel(model);
			obj_model->SetAABB(aabb);

			auto sym = std::make_shared<Symbol>();
			sym->SetModel(obj_model);

			sym->SetFilepath(filenames[i].ToStdString());

			AddItem(sym);
		}
	}
}

}