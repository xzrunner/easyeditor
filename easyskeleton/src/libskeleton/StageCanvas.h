#ifndef _LIBSKELETON_STAGE_CANVAS_H_
#define _LIBSKELETON_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

#include <memory>

class wxGLContext;

namespace s2 { class Skeleton; }

namespace libskeleton
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* editpanel, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	const s2::Skeleton& m_sk;

}; // StageCanvas

}

#endif // _LIBSKELETON_STAGE_CANVAS_H_