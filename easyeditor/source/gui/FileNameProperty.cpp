#include "FileNameProperty.h"
#include "FileHelper.h"

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(FileNameProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

FileNameProperty::FileNameProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, FileHelper::GetFilename(value.ToStdString()))
	, m_filepath(value)
{
}

bool FileNameProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	std::string command = "easycomplex.exe " + m_filepath;
	wxExecute(command, wxEXEC_ASYNC, NULL);
	return false;
}

void FileNameProperty::SetFilepath(const std::string& filepath)
{
	m_filepath = filepath;
	SetValue(FileHelper::GetFilename(filepath));
}

}