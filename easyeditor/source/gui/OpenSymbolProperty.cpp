#include "OpenSymbolProperty.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "Exception.h"
#include "ExceptionDlg.h"
#include "OpenSymbolMonitor.h"
#include "FileHelper.h"

#include <wx/filedlg.h>

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(OpenSymbolProperty,wxLongStringProperty,wxSize,const wxSize&,TextCtrlAndButton)

OpenSymbolProperty::OpenSymbolProperty(const wxString& label, const wxString& name, const wxString& value) 
	: wxLongStringProperty(label, name, value)
	, m_lsn(NULL)
	, m_parent(NULL)
{}

OpenSymbolProperty::~OpenSymbolProperty() 
{
	if (m_lsn) {
		m_lsn->RemoveReference();
	}
}

wxVariant OpenSymbolProperty::ChildChanged(wxVariant& thisValue,
                                        int childIndex,
                                        wxVariant& childValue ) const
{
// 	if (m_color) {
// //		return wxColour(m_color->r, m_color->g, m_color->b, m_color->a);
// 		return wxNullColour;
// 	} else {
		return wxNullColour;
//	}
}

void OpenSymbolProperty::RefreshChildren()
{
	//     if ( !GetChildCount() ) return;
	//     const wxSize& size = wxSizeRefFromVariant(m_value);
	//     Item(0)->SetValue( (long)size.x );
	//     Item(1)->SetValue( (long)size.y );
}

bool OpenSymbolProperty::OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
{
	if (!m_lsn) {
		return false;
	}

	std::string filepath = GetValue();
	std::string dirpath = FileHelper::GetFileDir(filepath);
	wxFileDialog dlg(m_parent, wxT("Open Symbol"), dirpath, filepath, "*.json", wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string sym_path = dlg.GetPath();
		try {
			Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(sym_path);
			m_lsn->OnOpenSymbol(symbol);
			symbol->RemoveReference();
		} catch (Exception& e) {
			ExceptionDlg dlg(m_parent, e);
			dlg.ShowModal();
		}
	}

	return false;
}

void OpenSymbolProperty::SetListener(OpenSymbolMonitor* lsn) 
{ 
	obj_assign(m_lsn, lsn);
}

}