#ifndef _EASYSKETCH_TRANSLATE_CAMERA_STATE_H_
#define _EASYSKETCH_TRANSLATE_CAMERA_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace esketch
{

class TranslateCameraState : public IEditState
{
public:
	TranslateCameraState(e3d::StageCanvas* canvas);

	virtual void OnMousePress(const sm::ivec2& pos) override;
	virtual void OnMouseRelease(const sm::ivec2& pos) override;
	virtual void OnMouseMove(const sm::ivec2& pos) override;

private:
	e3d::StageCanvas* m_canvas;

	bool m_is_open;

	sm::ivec2 m_last_pos;

}; // TranslateCameraState

}

#endif // _EASYSKETCH_TRANSLATE_CAMERA_STATE_H_