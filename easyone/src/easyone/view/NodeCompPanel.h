#pragma once

#include <wx/panel.h>

namespace eone
{

class NodeCompPanel : public wxPanel
{
public:
	NodeCompPanel(wxWindow* parent)
		: wxPanel(parent, wxID_ANY)
	{}
	virtual void RefreshNodeComp() = 0;

}; // NodeCompPanel

}