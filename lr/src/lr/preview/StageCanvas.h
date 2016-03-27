#ifndef _LR_PREVIEW_STAGE_CANVAS_H_
#define _LR_PREVIEW_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class PlayControl; class Sprite; }

namespace lr
{
namespace preview
{

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
		const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	ee::PlayControl& m_control;

	std::vector<const ee::Sprite*> m_sprites;

}; // StageCanvas

}
}

#endif // _LR_PREVIEW_STAGE_CANVAS_H_