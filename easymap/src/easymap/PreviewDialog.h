#ifndef _EASYMAP_PREVIEW_DIALOG_H_
#define _EASYMAP_PREVIEW_DIALOG_H_



namespace emap
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, int width, int height, 
		const std::vector<const ee::Sprite*>& sprites);

private:
	void InitLayout();

private:
	std::vector<const ee::Sprite*> m_sprites;

	ee::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYMAP_PREVIEW_DIALOG_H_
