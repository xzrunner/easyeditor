#ifndef _EASYUI_WINDOW_PREVIEW_CANVAS_H_
#define _EASYUI_WINDOW_PREVIEW_CANVAS_H_



namespace eui
{
namespace window
{

class PreviewCanvas : public ee::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control, 
		const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	ee::Matrix m_scale_mt;

	ee::PlayControl& m_control;

	std::vector<const ee::Sprite*> m_sprites;

}; // PreviewCanvas

}
}

#endif // _EASYUI_WINDOW_PREVIEW_CANVAS_H_