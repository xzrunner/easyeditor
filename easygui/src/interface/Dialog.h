#ifndef _EASYGUI_DIALOG_H_
#define _EASYGUI_DIALOG_H_

namespace egui
{

class Dialog
{
public:
	virtual ~Dialog() {}
	virtual void ShowModal() = 0;

}; // Dialog

}

#endif // _EASYGUI_DIALOG_H_