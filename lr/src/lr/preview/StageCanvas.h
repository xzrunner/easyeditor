#ifndef _LR_PREVIEW_STAGE_CANVAS_H_
#define _LR_PREVIEW_STAGE_CANVAS_H_



namespace lr
{
namespace preview
{

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
		const std::vector<const ee::Sprite*>& sprites);

protected:
	virtual void OnDrawSprites() const;

	virtual void OnTimer();

private:
	ee::PlayControl& m_control;

	std::vector<const ee::Sprite*> m_sprites;

}; // StageCanvas

}
}

#endif // _LR_PREVIEW_STAGE_CANVAS_H_