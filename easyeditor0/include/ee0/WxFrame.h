#pragma once

#include <wx/frame.h>

namespace ee0
{

class WxFrame : public wxFrame
{
public:
	WxFrame(const std::string& title, bool maxmize = false,
		const wxSize& size = wxSize(800, 600));

private:
	

}; // WxFrame

}