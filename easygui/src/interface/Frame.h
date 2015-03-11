#ifndef _EASYGUI_FRAME_H_
#define _EASYGUI_FRAME_H_

#include "Window.h"

namespace egui
{

class Frame : public Window
{
public:
	virtual ~Frame() {}
	virtual void Show() = 0;

}; // Frame

}

#endif // _EASYGUI_FRAME_H_