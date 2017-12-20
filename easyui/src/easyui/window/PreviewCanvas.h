#ifndef _EASYUI_WINDOW_PREVIEW_CANVAS_H_
#define _EASYUI_WINDOW_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

#include <SM_Matrix2D.h>
#include <sprite2/pre_defined.h>

namespace ee { class PlayControl; }

namespace eui
{
namespace window
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control, 
		const std::vector<ee::SprPtr>& sprs, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const override;

	virtual void OnTimer() override;

private:
	S2_MAT m_scale_mt;

	ee::PlayControl& m_control;

	std::vector<ee::SprPtr> m_sprs;

}; // PreviewCanvas

}
}

#endif // _EASYUI_WINDOW_PREVIEW_CANVAS_H_