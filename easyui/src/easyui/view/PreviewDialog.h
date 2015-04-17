#ifndef _EASYUI_PREVIEW_DIALOG_H_
#define _EASYUI_PREVIEW_DIALOG_H_

#include <drag2d.h>

namespace eui
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

#endif // _EASYUI_PREVIEW_DIALOG_H_
