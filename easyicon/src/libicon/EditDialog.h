#ifndef _EASYICON_EDIT_DIALOG_H_
#define _EASYICON_EDIT_DIALOG_H_



namespace eicon
{

class Symbol;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* sprite_impl);
	void InitEditOP(ee::Sprite* edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYICON_EDIT_DIALOG_H_