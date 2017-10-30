#ifndef _EASYSHAPE_ETID_DIALOG_SIMPLE_H_
#define _EASYSHAPE_ETID_DIALOG_SIMPLE_H_

#include <ee/OneFloatValueStatic.h>
#include <ee/Sprite.h>

#include <wx/dialog.h>

class wxGLContext;

namespace ee { class MultiSpritesImpl; }

namespace eshape
{

class Symbol;
class StagePanel;

class EditDialogSimple : public wxDialog
{
public:
	EditDialogSimple(wxWindow* parent, wxGLContext* glctx,
		ee::SprPtr edited, const ee::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialogSimple();

private:
	void InitLayout(wxGLContext* glctx, const ee::SprPtr& edited, 
		const ee::MultiSpritesImpl* sprite_impl);
	void InitEditOP(const ee::SprPtr& edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	ee::OneFloatValueStatic m_capture;

	DECLARE_EVENT_TABLE()

}; // EditDialogSimple

}

#endif // _EASYSHAPE_ETID_DIALOG_SIMPLE_H_
