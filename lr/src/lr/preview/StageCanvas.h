#ifndef _LR_PREVIEW_STAGE_CANVAS_H_
#define _LR_PREVIEW_STAGE_CANVAS_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
		const std::vector<const d2d::ISprite*>& sprites);

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

}; // StageCanvas

}
}

#endif // _LR_PREVIEW_STAGE_CANVAS_H_