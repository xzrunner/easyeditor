#ifndef _EASYUI_WINDOW_PREVIEW_CANVAS_H_
#define _EASYUI_WINDOW_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <SM_Matrix.h>

namespace ee { class PlayControl; class Sprite; }

namespace eui
{
namespace window
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control, 
		const std::vector<ee::Sprite*>& sprs, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	S2_MAT m_scale_mt;

	ee::PlayControl& m_control;

	std::vector<ee::Sprite*> m_sprs;

}; // PreviewCanvas

}
}

#endif // _EASYUI_WINDOW_PREVIEW_CANVAS_H_