#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_


#include <easy3d.h>

namespace eanim3d
{

class StagePanel;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawSprites() const;

private:
	void DrawBackground() const;
	void DrawSprites() const;

private:
	ee::MultiSpritesImpl* m_sprites_impl;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_