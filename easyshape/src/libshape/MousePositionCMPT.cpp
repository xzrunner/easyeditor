#include "MousePositionCMPT.h"
#include "MousePositionOP.h"

namespace libshape
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const wxString& name, 
									 d2d::EditPanel* editPanel)
	: d2d::MousePositionCMPT(parent, name, editPanel, false)
{
	m_editOP = new MousePositionOP(editPanel, editPanel->GetStageImpl(), this);
}

}