#ifndef _LR_PREVIEW_STAGE_CANVAS_H_
#define _LR_PREVIEW_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::PlayControl& control,
		const std::vector<const d2d::Sprite*>& sprites);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	d2d::PlayControl& m_control;

	std::vector<const d2d::Sprite*> m_sprites;

}; // StageCanvas

}
}

#endif // _LR_PREVIEW_STAGE_CANVAS_H_