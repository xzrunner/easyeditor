#ifndef _LIBSHAPE_ETID_DIALOG_SIMPLE_H_
#define _LIBSHAPE_ETID_DIALOG_SIMPLE_H_

#include <drag2d.h>

namespace libshape
{

class Symbol;
class StagePanel;

class EditDialogSimple : public wxDialog
{
public:
	EditDialogSimple(wxWindow* parent, wxGLContext* glctx,
		d2d::Sprite* edited, const d2d::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialogSimple();

private:
	void InitLayout(wxGLContext* glctx, d2d::Sprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);
	void InitEditOP(d2d::Sprite* edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	d2d::OneFloatValueStatic m_capture;

	DECLARE_EVENT_TABLE()

}; // EditDialogSimple

}

#endif // _LIBSHAPE_ETID_DIALOG_SIMPLE_H_
