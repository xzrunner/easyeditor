#ifndef _EASY_EASYCOMPLEX_PREVIEW_DIALOG_H_
#define _EASY_EASYCOMPLEX_PREVIEW_DIALOG_H_

#include <ee/PlayControl.h>

#include <wx/dialog.h>

#include <vector>

class wxGLContext;

namespace ee { class Sprite; }

namespace ecomplex
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, 
		const std::vector<const ee::Sprite*>& sprites);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildEditPanel(wxSizer* sizer, wxGLContext* glctx);

private:
	std::vector<const ee::Sprite*> m_sprites;

	ee::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASY_EASYCOMPLEX_PREVIEW_DIALOG_H_