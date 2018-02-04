#include "ee0/WxLibraryImagePage.h"
#include "ee0/WxLibraryList.h"
#include "ee0/WxLibraryItem.h"

namespace ee0
{

WxLibraryImagePage::WxLibraryImagePage(wxWindow* parent)
	: WxLibraryPage(parent, "Image")
{
}

void WxLibraryImagePage::OnAddPress(wxCommandEvent& event)
{
	std::string formatFilter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose images"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (auto& filename : filenames) {
			m_list->Insert(std::make_shared<WxLibraryItem>(filename.ToStdString()));
		}
	}
}

}