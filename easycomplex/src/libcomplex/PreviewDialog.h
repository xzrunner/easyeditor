#ifndef _EASYCOMPLEX_PREVIEW_DIALOG_H_
#define _EASYCOMPLEX_PREVIEW_DIALOG_H_



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

#endif // _EASYCOMPLEX_PREVIEW_DIALOG_H_