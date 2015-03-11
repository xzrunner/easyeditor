#ifndef _EASYGUI_DIALOG_WX_H_
#define _EASYGUI_DIALOG_WX_H_

#include "interface/Dialog.h"

#include <wx/dialog.h>

namespace egui
{

class Window;

class DialogWX : public egui::Dialog
{
public:
	DialogWX(Window* parent, const std::string& title);
	virtual ~DialogWX();

	virtual void ShowModal();

private:
	wxDialog* m_impl;

}; // DialogWX

}

#endif // _EASYGUI_DIALOG_WX_H_