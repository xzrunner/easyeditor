#include "UniversalCMPT.h"

namespace d2d
{

UniversalCMPT::UniversalCMPT(wxWindow* parent, const wxString& name,
							 EditPanelImpl* stage, AbstractEditOP* editOP)
	: AbstractEditCMPT(parent, name, stage)
{
	m_editOP = editOP;
}

wxSizer* UniversalCMPT::initLayout()
{
	return new wxBoxSizer(wxHORIZONTAL);
}

}