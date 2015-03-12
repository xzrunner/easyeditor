#ifndef _EASYGUI_PANEL_H_
#define _EASYGUI_PANEL_H_

#include "Window.h"

namespace egui
{

class PanelImpl;

class Panel : public Window
{
public:
	Panel(Window* parent, int w, int h);
	virtual ~Panel();

protected:
	virtual void OnSize(int w, int h);

private:
	PanelImpl* m_impl;

	friend class PanelImpl;

}; // Panel

}

#endif // _EASYGUI_PANEL_H_