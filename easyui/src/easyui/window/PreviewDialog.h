#ifndef _EASYUI_WINDOW_PREVIEW_DIALOG_H_
#define _EASYUI_WINDOW_PREVIEW_DIALOG_H_

#include <ee/PlayControl.h>

#include <wx/dialog.h>

#include <vector>

class wxGLContext;

namespace ee { class Sprite; }

namespace eui
{
namespace window
{

class PreviewDialog : public wxDialog
{
public:
	PreviewDialog(wxWindow* parent, wxGLContext* glctx, int width, 
		int height, const std::vector<const ee::Sprite*>& sprites);

private:
	void InitLayout(wxGLContext* glctx);

private:
	std::vector<const ee::Sprite*> m_sprites;

	ee::PlayControl m_control;

}; // PreviewDialog

}
}

#endif // _EASYUI_PREVIEW_DIALOG_H_
