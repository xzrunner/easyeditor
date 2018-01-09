#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace ee { class MultiSpritesImpl; }

namespace eanim3d
{

class StagePanel;

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl);

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

private:
	void DrawBackground() const;
	void DrawSprites() const;

private:
	ee::MultiSpritesImpl* m_sprites_impl;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_