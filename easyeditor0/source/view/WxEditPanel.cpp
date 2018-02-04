#include "ee0/WxEditPanel.h"
#include "ee0/EditPanelImpl.h"

namespace ee0
{

BEGIN_EVENT_TABLE(WxEditPanel, wxPanel)
 	EVT_SIZE(WxEditPanel::OnSize)
END_EVENT_TABLE()

WxEditPanel::WxEditPanel(wxWindow* parent)
	: wxPanel(parent)
{
	m_impl = std::make_unique<EditPanelImpl>();
}

bool WxEditPanel::GetKeyState(int key) const
{
	return m_impl->GetKeyState(key);
}

void WxEditPanel::OnSize(wxSizeEvent& event)
{
	m_impl->OnSize(event);
}

}