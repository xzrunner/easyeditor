#ifndef _EASYMAP_PREVIEW_CANVAS_H_
#define _EASYMAP_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace emap
{

class PreviewCanvas : public d2d::DynamicStageCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
		const std::vector<const d2d::ISprite*>& sprites);

protected:
	virtual void initGL();
	virtual void onDraw();

	virtual void OnTimer();

private:
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASYMAP_PREVIEW_CANVAS_H_