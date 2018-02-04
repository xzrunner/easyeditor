#include "ee0/WxFrame.h"

namespace ee0
{

WxFrame::WxFrame(const std::string& title, bool maxmize, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE | (wxMAXIMIZE * maxmize))
{
}

}