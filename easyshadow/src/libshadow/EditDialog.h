#ifndef _EASYSHADOW_EDIT_DIALOG_H_
#define _EASYSHADOW_EDIT_DIALOG_H_

#include <wx/dialog.h>

class wxGLContext;

namespace ee { class MultiSpritesImpl; class Sprite; class Camera; class EditPanel; }

namespace eshadow
{

class Symbol;
class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		Sprite* edited, const ee::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

	void InitCamera(const ee::SprPtr& spr) const;

private:
	std::shared_ptr<Symbol> m_sym;

	ee::EditPanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYSHADOW_EDIT_DIALOG_H_