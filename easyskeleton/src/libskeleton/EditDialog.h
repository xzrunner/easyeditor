#ifndef _LIBSKELETON_EDIT_DIALOG_H_
#define _LIBSKELETON_EDIT_DIALOG_H_

#include <wx/dialog.h>

class wxGLContext;

namespace libskeleton
{

class Sprite;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx, Sprite* spr);

private:
	void InitLayout(wxGLContext* glctx, Sprite* spr);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _LIBSKELETON_EDIT_DIALOG_H_