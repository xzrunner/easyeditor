#ifndef _EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASYCOMPLEX_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
		const std::vector<const d2d::ISprite*>& sprites);

protected:
	virtual void InitGL();
	virtual void OnDrawSprites() const;

protected:
	virtual void OnTimer();

private:
	void drawStageData();

//	void getCurrSprites(std::vector<d2d::ISprite*>& sprites) const;

private:
	d2d::PlayControl& m_control;

	std::vector<const d2d::ISprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASYCOMPLEX_PREVIEW_CANVAS_H_