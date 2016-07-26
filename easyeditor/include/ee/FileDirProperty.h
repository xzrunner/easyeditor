#ifndef _EASYEDITOR_FILE_DIR_PROPERTY_H_
#define _EASYEDITOR_FILE_DIR_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class FileDirProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(FileDirProperty)

public:
	FileDirProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, 
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value);

private:
	wxWindow* m_parent;

	wxString m_dir;

}; // FileDirProperty

}

#endif // _EASYEDITOR_FILE_DIR_PROPERTY_H_