#ifndef _EASY_EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASY_EASYCOMPLEX_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace ee { class Sprite; }

namespace ecomplex
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	void drawStageData();

//	void getCurrSprites(std::vector<ee::Sprite*>& sprites) const;

private:
	std::vector<const ee::Sprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASY_EASYCOMPLEX_PREVIEW_CANVAS_H_