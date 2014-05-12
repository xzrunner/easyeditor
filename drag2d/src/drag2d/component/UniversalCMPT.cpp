#include "UniversalCMPT.h"

namespace d2d
{

UniversalCMPT::UniversalCMPT(wxWindow* parent, const wxString& name,
							 EditPanel* editPanel, AbstractEditOP* editOP)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = editOP;
}

wxSizer* UniversalCMPT::initLayout()
{
	return new wxBoxSizer(wxHORIZONTAL);
}

} // d2d