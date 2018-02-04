#pragma once

#include "ee0/EditPanelImpl.h"

#include <wx/wx.h>

#include <memory>

namespace ee0
{

class WxEditPanel : public wxPanel
{
public:
	WxEditPanel(wxWindow* parent);
	~WxEditPanel() = default;

	EditPanelImpl& GetImpl() { return *m_impl; }
	const EditPanelImpl& GetImpl() const { return *m_impl; }

	bool GetKeyState(int key) const;

private:
	void OnSize(wxSizeEvent& event);

private:
	std::unique_ptr<EditPanelImpl> m_impl;

	DECLARE_EVENT_TABLE()

}; // WxEditPanel

}