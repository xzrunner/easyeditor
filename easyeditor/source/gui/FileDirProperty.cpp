#include "FileDirProperty.h"

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(FileDirProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

FileDirProperty::FileDirProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, value)
	, m_dir(value)
{
}

bool FileDirProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	std::string command = "explorer " + m_dir;
	wxExecute(command, wxEXEC_ASYNC, NULL);
	return false;
}

}