#ifndef _EASYCOMPLEX_PREVIEW_DIALOG_H_
#define _EASYCOMPLEX_PREVIEW_DIALOG_H_

#include <ee/PlayControl.h>
#include <ee/Sprite.h>

#include <wx/dialog.h>

#include <vector>

class wxGLContext;

namespace ecomplex
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, 
		const std::vector<ee::SprPtr>& sprs);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildEditPanel(wxSizer* sizer, wxGLContext* glctx);

private:
	std::vector<ee::SprPtr> m_sprs;

	ee::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYCOMPLEX_PREVIEW_DIALOG_H_