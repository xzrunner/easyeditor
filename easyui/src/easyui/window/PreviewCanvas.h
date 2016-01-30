#ifndef _EASYUI_WINDOW_PREVIEW_CANVAS_H_
#define _EASYUI_WINDOW_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::PlayControl& control, 
		const std::vector<const d2d::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	d2d::Matrix m_scale_mt;

	d2d::PlayControl& m_control;

	std::vector<const d2d::Sprite*> m_sprites;

}; // PreviewCanvas

}
}

#endif // _EASYUI_WINDOW_PREVIEW_CANVAS_H_