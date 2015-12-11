#ifndef _EASYCOMPLEX_PREVIEW_DIALOG_H_
#define _EASYCOMPLEX_PREVIEW_DIALOG_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, 
		const std::vector<const d2d::ISprite*>& sprites);
	~PreviewDialog();

private:
	void InitLayout(wxGLContext* glctx);

	void BuildEditPanel(wxSizer* sizer, wxGLContext* glctx);

private:
	std::vector<const d2d::ISprite*> m_sprites;

	d2d::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYCOMPLEX_PREVIEW_DIALOG_H_