#ifndef _EASYGUI_WINDOW_H_
#define _EASYGUI_WINDOW_H_

namespace egui
{

class Window
{
public:
	virtual ~Window() {}
	virtual void* GetRawPtr() = 0;

}; // Window

}

#endif // _EASYGUI_WINDOW_H_