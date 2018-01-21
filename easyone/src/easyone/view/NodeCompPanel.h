#pragma once

#include <wx/panel.h>

namespace eone
{

class NodeCompPanel : public wxPanel
{
public:
	NodeCompPanel(wxWindow* parent);

private:
	void InitLayout();

}; // NodeCompPanel

}