#ifndef _EASYGUI_WINDOW_WX_H_
#define _EASYGUI_WINDOW_WX_H_

#include "interface/Window.h"

#include <wx/window.h>

namespace egui
{

class WindowWX : public egui::Window, public wxWindow
{
public:	
	
}; // WindowWX

}

#endif // _EASYGUI_WINDOW_WX_H_