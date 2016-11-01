#ifndef _EASYEDITOR_FILE_NAME_PROPERTY_H_
#define _EASYEDITOR_FILE_NAME_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class FileNameProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(FileNameProperty)

public:
	FileNameProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, 
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value);

	void SetFilepath(const std::string& filepath);

private:
	wxString m_filepath;

}; // FileNameProperty

}

#endif // _EASYEDITOR_FILE_NAME_PROPERTY_H_