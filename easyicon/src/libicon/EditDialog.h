#ifndef _EASYICON_EDIT_DIALOG_H_
#define _EASYICON_EDIT_DIALOG_H_

#include <ee/Sprite.h>

#include <wx/dialog.h>

class wxGLContext;

namespace ee { class MultiSpritesImpl; }

namespace eicon
{

class Symbol;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* sprite_impl);
	void InitEditOP(const ee::SprPtr& edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYICON_EDIT_DIALOG_H_