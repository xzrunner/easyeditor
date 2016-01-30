#ifndef _EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASYCOMPLEX_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
		const std::vector<const d2d::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	void drawStageData();

//	void getCurrSprites(std::vector<d2d::Sprite*>& sprites) const;

private:
	std::vector<const d2d::Sprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASYCOMPLEX_PREVIEW_CANVAS_H_