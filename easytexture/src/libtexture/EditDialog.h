#ifndef _EASYTEXTURE_EDIT_DIALOG_H_
#define _EASYTEXTURE_EDIT_DIALOG_H_

#include <ee/Sprite.h>

#include <wx/dialog.h>

#include <memory>

class wxGLContext;

namespace ee { class MultiSpritesImpl; class EditPanel; }

namespace etexture
{

class Symbol;
class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx, 
		const std::shared_ptr<Sprite>& edited, const ee::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

	void InitCamera(const ee::SprPtr& spr) const;

private:
	std::shared_ptr<Symbol> m_sym;

	ee::EditPanel* m_stage;

	bool m_visible_tex_edge;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTEXTURE_EDIT_DIALOG_H_