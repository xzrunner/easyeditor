#ifndef _LIBSKELETON_STAGE_CANVAS_H_
#define _LIBSKELETON_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

class wxGLContext;

namespace libskeleton
{

class StagePanel;
class Sprite;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editpanel, wxGLContext* glctx);
	virtual ~StageCanvas();

protected:
	virtual void OnDrawSprites() const;

private:
	const Sprite* m_spr;

}; // StageCanvas

}

#endif // _LIBSKELETON_STAGE_CANVAS_H_