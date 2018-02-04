#pragma once

#include <wx/collpane.h>

namespace ee0
{

class WxCompPanel : public wxCollapsiblePane
{
public:
	WxCompPanel(wxWindow* parent, const std::string& title);
	virtual void RefreshNodeComp() = 0;

private:
	enum
	{
		ID_NODE_COMP_PANEL = 2000
	};

private:
	void OnCollapsChanged(wxCollapsiblePaneEvent& event);

	DECLARE_EVENT_TABLE()

}; // WxCompPanel

}