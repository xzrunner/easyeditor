#ifndef _EASYCOMPLEX_PREVIEW_CANVAS_H_
#define _EASYCOMPLEX_PREVIEW_CANVAS_H_

#include <ee/CameraCanvas.h>
#include <ee/Sprite.h>

namespace ecomplex
{

class PreviewCanvas : public ee::CameraCanvas
{
public:
	PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		const std::vector<ee::SprPtr>& sprs, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	void drawStageData();

//	void getCurrSprites(std::vector<ee::SprPtr>& sprs) const;

private:
	std::vector<ee::SprPtr> m_sprs;

}; // PreviewCanvas

}

#endif // _EASYCOMPLEX_PREVIEW_CANVAS_H_