#ifndef _EASYGUI_FRAME_H_
#define _EASYGUI_FRAME_H_

#include "Window.h"

#include <string>

namespace egui
{

class FrameImpl;

class Frame : public Window
{
public:
	Frame(const std::string& title);
	virtual ~Frame();

	//
	// interface Window
	//
	virtual void* GetRawPtr();

	virtual void Show();

private:
	FrameImpl* m_impl;

}; // Frame

}

#endif // _EASYGUI_FRAME_H_