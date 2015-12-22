#ifndef _EASYUI_WINDOW_PREVIEW_DIALOG_H_
#define _EASYUI_WINDOW_PREVIEW_DIALOG_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, int width, 
		int height, const std::vector<const d2d::ISprite*>& sprites);

private:
	void InitLayout(wxGLContext* glctx);

private:
	std::vector<const d2d::ISprite*> m_sprites;

	d2d::PlayControl m_control;

}; // PreviewDialog

}
}

#endif // _EASYUI_PREVIEW_DIALOG_H_
