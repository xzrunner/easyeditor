#ifndef _EASYGUI_DIALOG_H_
#define _EASYGUI_DIALOG_H_

#include <string>

namespace egui
{

class Window;
class DialogImpl;

class Dialog
{
public:
	Dialog(Window* parent, const std::string& title, int w, int h);
	virtual ~Dialog();

	virtual void ShowModal();

private:
	DialogImpl* m_impl;

}; // Dialog

}

#endif // _EASYGUI_DIALOG_H_