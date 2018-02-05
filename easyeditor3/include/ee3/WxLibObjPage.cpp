#include "ee3/WxLibObjPage.h"

#include <ee0/WxLibraryList.h>
#include <ee0/WxLibraryItem.h>

#include <sprite2/ModelSymbol.h>
#include <sprite2/SymType.h>

namespace ee3
{

WxLibObjPage::WxLibObjPage(wxWindow* parent)
	: ee0::WxLibraryPage(parent, "Model")
{
//	m_list->SetFileter(ModelFile::Instance()->Tag(n3::MODEL_OBJECT));
}

void WxLibObjPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("choose model files"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);

		for (size_t i = 0, n = filenames.size(); i < n; ++i) {
			m_list->Insert(std::make_shared<ee0::WxLibraryItem>(filenames[i].ToStdString()));
		}
	}
}

}