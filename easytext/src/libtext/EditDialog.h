#ifndef _EASYTEXT_EDIT_DIALOG_H_
#define _EASYTEXT_EDIT_DIALOG_H_



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
		Sprite* spr, const ee::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, Sprite* spr, const ee::MultiSpritesImpl* sprite_impl);

	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent, wxGLContext* glctx,
		Sprite* spr, const ee::MultiSpritesImpl* sprite_impl);
	wxWindow* InitLayoutRight(wxWindow* parent, Sprite* spr);

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