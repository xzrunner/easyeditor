#include "ee3/LibObjPage.h"
#include "ee3/NodeSymbol.h"

#include <ee/LibraryList.h>

#include <sprite2/ModelSymbol.h>
#include <sprite2/SymType.h>
#include <node3/AssimpHelper.h>
#include <node3/Model.h>
#include <node3/AABB.h>
#include <node3/CompModel.h>
#include <node3/CompAABB.h>

namespace ee3
{

LibObjPage::LibObjPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Model")
{
	InitLayout();
//	m_list->SetFileter(ModelFile::Instance()->Tag(n3::MODEL_OBJECT));
}

bool LibObjPage::IsHandleSymbol(const ee::SymPtr& sym) const
{
	return sym->Type() == s2::SYM_MODEL;
}

void LibObjPage::OnAddPress(wxCommandEvent& event)
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

			auto node = std::make_shared<n0::SceneNode>();

			auto& cmodel = node->AddComponent<n3::CompModel>();
			cmodel.SetModel(model);

			auto& caabb = node->AddComponent<n3::CompAABB>();
			caabb.SetAABB(aabb);

			auto sym = std::make_shared<NodeSymbol>();
			sym->SetNode(node);

			sym->SetFilepath(filenames[i].ToStdString());

			AddItem(sym);
		}
	}
}

}