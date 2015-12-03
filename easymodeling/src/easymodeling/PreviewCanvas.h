#ifndef _EASYMODELING_PREVIEW_CANVAS_H_
#define _EASYMODELING_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace emodeling
{
class PreviewPanel;

class PreviewCanvas : public d2d::OrthoCanvas
{
public:
	PreviewCanvas(PreviewPanel* editPanel);
	virtual ~PreviewCanvas();

protected:
	virtual void OnDrawSprites() const;

private:
	static const int FRAME_RATE = 60;

private:
	PreviewPanel* m_stage_panel;

}; // PreviewCanvas

}

#endif // _EASYMODELING_PREVIEW_CANVAS_H_
