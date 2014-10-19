#ifndef _LIBSKETCH_TRANSLATE_CAMERA_STATE_H_
#define _LIBSKETCH_TRANSLATE_CAMERA_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class TranslateCameraState : public IEditState
{
public:
	TranslateCameraState(e3d::StageCanvas* canvas);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);

private:
	e3d::StageCanvas* m_canvas;

	bool m_is_open;

	ivec2 m_last_pos;

}; // TranslateCameraState

}

#endif // _LIBSKETCH_TRANSLATE_CAMERA_STATE_H_