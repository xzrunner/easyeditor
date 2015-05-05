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
	EditDialogSimple(wxWindow* parent, d2d::ISprite* edited,
		const std::vector<d2d::ISprite*>& bg_sprites);
	virtual ~EditDialogSimple();

private:
	void InitLayout(d2d::ISprite* edited, const std::vector<d2d::ISprite*>& bg_sprites);
	void InitEditOP(d2d::ISprite* edited);

	void OnClose(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	d2d::OneFloatValueStatic m_capture;

	DECLARE_EVENT_TABLE()

}; // EditDialogSimple

}

#endif // _LIBSHAPE_ETID_DIALOG_SIMPLE_H_
