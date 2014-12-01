#include "RegularToolbarPage.h"
#include "StagePanel.h"

#include <easypacker.h>

namespace epacker
{

RegularToolbarPage::RegularToolbarPage(wxWindow* parent, StagePanel* stage)
	: wxPanel(parent)
	, m_stage(stage)
{
	InitLayout();
}

void RegularToolbarPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	// dir
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("src dir"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_src_dir = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxTE_READONLY);
		sz->Add(m_src_dir);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(RegularToolbarPage::OnSetSrcDir));
		sz->Add(btn);

		sizer->Add(sz);
	}
	SetSizer(sizer);
}

void RegularToolbarPage::OnSetSrcDir(wxCommandEvent& event)
{
	d2d::ZoomViewOP* op = static_cast<d2d::ZoomViewOP*>(m_stage->getEditOP());
	op->setMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Src Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		wxString dir = dlg.GetPath();

		wxArrayString files;
		d2d::FilenameTools::fetchAllFiles(dir.ToStdString(), files);
		libpacker::RegularRectPack rrp(files);

		m_src_dir->SetValue(dir);
	}

	op->setMouseMoveFocus(true);
}

}