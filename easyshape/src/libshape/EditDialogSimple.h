#ifndef _EASYSHAPE_ETID_DIALOG_SIMPLE_H_
#define _EASYSHAPE_ETID_DIALOG_SIMPLE_H_

#include <wx/dialog.h>

#include <ee/OneFloatValueStatic.h>

class wxGLContext;

namespace ee { class Sprite; class MultiSpritesImpl; }

namespace eshape
{

class Symbol;
class StagePanel;

class EditDialogSimple : public wxDialog
{
public:
	EditDialogSimple(wxWindow* parent, wxGLContext* glctx,
		ee::Sprite* edited, const ee::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialogSimple();

private:
	void InitLayout(wxGLContext* glctx, ee::Sprite* edited, 
		const ee::MultiSpritesImpl* sprite_impl);
	void InitEditOP(ee::Sprite* edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	ee::OneFloatValueStatic m_capture;

	DECLARE_EVENT_TABLE()

}; // EditDialogSimple

}

#endif // _EASYSHAPE_ETID_DIALOG_SIMPLE_H_
