#include "UniversalCMPT.h"

namespace ee
{

UniversalCMPT::UniversalCMPT(wxWindow* parent, const std::string& name,
							 EditPanelImpl* stage, EditOP* editOP)
	: EditCMPT(parent, name, stage)
{
	m_editop = editOP;
}

wxSizer* UniversalCMPT::InitLayout()
{
	return new wxBoxSizer(wxHORIZONTAL);
}

}