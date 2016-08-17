#ifndef _LR_MAIN_DIALOG_H_
#define _LR_MAIN_DIALOG_H_

#include <ee/PlayControl.h>

#include <wx/dialog.h>

#include <vector>

class wxGLContext;

namespace ee { class Sprite; }

namespace lr
{
namespace preview
{

class MainDialog : public wxDialog
{
public:
	MainDialog(wxWindow* parent, wxGLContext* glctx, int width, 
		int height, const std::vector<const ee::Sprite*>& sprs);

private:
	void InitLayout(wxGLContext* glctx);

private:
	std::vector<const ee::Sprite*> m_sprs;

	ee::PlayControl m_control;

}; // MainDialog

}
}

#endif // _LR_MAIN_DIALOG_H_
