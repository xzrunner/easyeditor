#include "UniversalCMPT.h"

namespace ee
{

UniversalCMPT::UniversalCMPT(wxWindow* parent, const std::string& name,
							 EditPanelImpl* stage, const std::shared_ptr<EditOP>& editop)
	: EditCMPT(parent, name, stage)
{
	m_editop = editop;
}

wxSizer* UniversalCMPT::InitLayout()
{
	return new wxBoxSizer(wxHORIZONTAL);
}

}