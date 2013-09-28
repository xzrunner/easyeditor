#pragma once

#include <wx/wx.h>

namespace d2d
{
	class AbstractWidget : public wxPanel
	{
	public:
		AbstractWidget(wxWindow* parent) : wxPanel(parent) {}
		virtual ~AbstractWidget() {}

	}; // AbstractWidget
}

