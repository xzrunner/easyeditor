#ifndef _EASYEDITOR_FILEPATH_PROPERTY_H_
#define _EASYEDITOR_FILEPATH_PROPERTY_H_

#include <wx/propgrid/propgrid.h>

namespace ee
{

class FilepathProperty : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(FilepathProperty)

public:
	FilepathProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL, 
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value);

	void SetFilter(const std::string& filter) { m_filter = filter; }

	void SetParent(wxWindow* parent) { m_parent = parent; }

private:
	wxWindow* m_parent;

	std::string m_filter;

	std::string m_filepath;

}; // FilepathProperty

}

#endif // _EASYEDITOR_FILEPATH_PROPERTY_H_