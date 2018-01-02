#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <sprite2/SymType.h>
#include <model3/AssimpHelper.h>
#include <model3/Model.h>
#include <model3/ObjectModel.h>

namespace eanim3d
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

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("choose model files"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);

		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			auto model = std::unique_ptr<m3::Model>(
				m3::AssimpHelper::Load(filenames[i].ToStdString()));
			auto obj_model = std::make_shared<m3::ObjectModel>();
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

}