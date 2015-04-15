#ifndef _EASYMAP_PREVIEW_DIALOG_H_
#define _EASYMAP_PREVIEW_DIALOG_H_

#include <drag2d.h>

namespace emap
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, int width, int height, 
		const std::vector<const d2d::ISprite*>& sprites);

private:
	void InitLayout();

private:
	std::vector<const d2d::ISprite*> m_sprites;

	d2d::PlayControl m_control;

}; // PreviewDialog

}

#endif // _EASYMAP_PREVIEW_DIALOG_H_
