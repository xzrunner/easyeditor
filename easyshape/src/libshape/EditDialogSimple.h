#ifndef _LIBSHAPE_ETID_DIALOG_SIMPLE_H_
#define _LIBSHAPE_ETID_DIALOG_SIMPLE_H_

#include <wx/wx.h>

#include "NodeCaptureStatic.h"

namespace libshape
{

class Symbol;
class StagePanel;

class EditDialogSimple : public wxDialog
{
public:
	EditDialogSimple(wxWindow* parent, Symbol* symbol);
	virtual ~EditDialogSimple();

private:
	void InitLayout(Symbol* symbol);
	void InitEditOP(Symbol* symbol);

	void OnClose(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	NodeCaptureStatic m_capture;

	DECLARE_EVENT_TABLE()

}; // EditDialogSimple

}

#endif // _LIBSHAPE_ETID_DIALOG_SIMPLE_H_
