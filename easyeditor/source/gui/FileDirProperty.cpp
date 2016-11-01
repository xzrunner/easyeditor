#include "FileDirProperty.h"
#include "FileHelper.h"

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(FileDirProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

FileDirProperty::FileDirProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, FileHelper::GetFileDir(value.ToStdString()))
	, m_filepath(value)
{
}

bool FileDirProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	std::string command = "explorer /select, " + m_filepath;
	wxExecute(command, wxEXEC_ASYNC, NULL);
	return false;
}

void FileDirProperty::SetFilepath(const std::string& filepath)
{
	m_filepath = filepath;
	SetValue(FileHelper::GetFileDir(filepath));
}

}