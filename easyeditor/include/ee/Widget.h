#ifndef _EASYEDITOR_WIDGET_H_
#define _EASYEDITOR_WIDGET_H_

#include <wx/panel.h>

namespace ee
{

class Widget : public wxPanel
{
public:
	Widget(wxWindow* parent) : wxPanel(parent) {}
	virtual ~Widget() {}

}; // Widget

}

#endif // _EASYEDITOR_WIDGET_H_