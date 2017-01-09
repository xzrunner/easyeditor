#include "FilepathProperty.h"
#include "FileHelper.h"

#include <wx/filedlg.h>

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(FilepathProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

FilepathProperty::FilepathProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, value)
	, m_parent(NULL)
	, m_filter(wxFileSelectorDefaultWildcardStr)
	, m_filepath(value)
	, m_cb(NULL)
	, m_ud(NULL)
{
}

bool FilepathProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	std::string dir = FileHelper::GetFileDir(m_filepath);
	wxFileDialog dlg(m_parent, "Select File", dir, m_filepath, m_filter);
	if (dlg.ShowModal() == wxID_OK) 
	{
		wxString filepath = dlg.GetPath();
		SetValue(filepath);
		if (m_cb) {
			m_cb(filepath.ToStdString(), m_ud);
		}
		return true;
	} 
	else 
	{
		return false;
	}
}

}