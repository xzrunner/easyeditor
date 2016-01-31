#ifndef _EASYMAP_PREVIEW_CANVAS_H_
#define _EASYMAP_PREVIEW_CANVAS_H_



namespace emap
{

class PreviewCanvas : public ee::OrthoCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::PlayControl& control, const std::vector<const ee::Sprite*>& sprites);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	ee::PlayControl& m_control;

	std::vector<const ee::Sprite*> m_sprites;

}; // PreviewCanvas

}

#endif // _EASYMAP_PREVIEW_CANVAS_H_