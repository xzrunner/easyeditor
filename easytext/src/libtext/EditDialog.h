#ifndef _EASYTEXT_EDIT_DIALOG_H_
#define _EASYTEXT_EDIT_DIALOG_H_

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace ee { class MultiSpritesImpl; class LibraryPanel; }

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
		const std::shared_ptr<Sprite>& spr, const ee::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, const std::shared_ptr<Sprite>& spr, const ee::MultiSpritesImpl* sprite_impl);

	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
		const std::shared_ptr<Sprite>& spr, const ee::MultiSpritesImpl* sprite_impl);
	wxWindow* InitLayoutRight(wxWindow* parent, const std::shared_ptr<Sprite>& spr);

	void OnCloseEvent(wxCloseEvent& event);

private:
	ee::LibraryPanel* m_library;

	StagePanel* m_stage;
	InputPanel* m_input;
	
	ToolbarPanel* m_toolbar;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTEXT_EDIT_DIALOG_H_