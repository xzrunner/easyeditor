#include "FilepathDialog.h"
#include "FileHelper.h"

namespace ee
{

std::string FilepathDialog::m_last_dir = "";

FilepathDialog::FilepathDialog(wxWindow* parent, const std::string& filepath)
	: BaseDialog(parent, "Filepath")
{
	InitLayout(filepath);
}

std::string FilepathDialog::GetFilepath() const
{
	return std::string(m_dir->GetValue().ToStdString().c_str()) 
		 + std::string("\\") 
		 + std::string(m_name->GetValue().ToStdString().c_str());
}

void FilepathDialog::SaveLastDir()
{
	m_last_dir = m_dir->GetValue().ToStdString().c_str();
}

void FilepathDialog::InitLayout(const std::string& filepath)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "dir"), 0, wxLEFT | wxRIGHT, 5);

		auto dir = FileHelper::GetFileDir(filepath);
		if (!m_last_dir.empty()) {
			dir = m_last_dir;
		}
		m_dir = new wxTextCtrl(this, wxID_ANY, dir.c_str(), wxDefaultPosition, wxSize(400, -1));
		sz->Add(m_dir, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "name"), 0, wxLEFT | wxRIGHT, 5);

		auto name = FileHelper::GetFilenameWithExtension(filepath);
		m_name = new wxTextCtrl(this, wxID_ANY, name.c_str(), wxDefaultPosition, wxSize(400, -1));
		sz->Add(m_name, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sz->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);
		sizer->Add(sz, 0, wxCENTER);
	}
	SetSizer(sizer);
	sizer->Fit(this);
}

}