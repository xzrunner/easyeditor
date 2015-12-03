#ifndef _EASYTEXT_EDIT_DIALOG_H_
#define _EASYTEXT_EDIT_DIALOG_H_

#include <drag2d.h>

namespace etext
{

class Sprite;
class StagePanel;
class InputPanel;
class ToolbarPanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx, 
		Sprite* spr, const d2d::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, Sprite* spr, const d2d::MultiSpritesImpl* sprite_impl);

	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
		Sprite* spr, const d2d::MultiSpritesImpl* sprite_impl);
	wxWindow* InitLayoutRight(wxWindow* parent, Sprite* spr);

	void OnCloseEvent(wxCloseEvent& event);

private:
	d2d::LibraryPanel* m_library;

	StagePanel* m_stage;
	InputPanel* m_input;
	
	ToolbarPanel* m_toolbar;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTEXT_EDIT_DIALOG_H_