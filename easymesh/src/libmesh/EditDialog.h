#ifndef _EASYMESH_EDIT_DIALOG_H_
#define _EASYMESH_EDIT_DIALOG_H_

#include <ee/Sprite.h>

#include <wx/dialog.h>

namespace ee { class EditPanel; class MultiSpritesImpl; }

class wxGLContext;

namespace emesh
{

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

private:
	ee::EditPanel* m_stage;

	std::shared_ptr<Sprite> m_spr;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYMESH_EDIT_DIALOG_H_