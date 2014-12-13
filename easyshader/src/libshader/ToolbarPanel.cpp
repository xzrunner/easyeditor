#include "ToolBarPanel.h"

namespace eshader
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	return sizer;
}

}