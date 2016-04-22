#ifndef _EASYEDITOR_CAMERA_MGR_H_
#define _EASYEDITOR_CAMERA_MGR_H_

#include "singleton.h"

namespace ee
{

class Camera;

class CameraMgr
{
public:
	enum Type
	{
		ORTHO = 0,
		PSEUDO3D,

		MAX_COUNT,
	};

public:
	void SetCamera(Type t);
	Camera* GetCamera() const;
	Camera* GetCamera(Type t) const;

	bool IsType(Type t) const;

private:
	Camera* m_cameras[MAX_COUNT];
	Type m_curr_cam;

	SINGLETON_DECLARATION(CameraMgr)

}; // CameraMgr

}

#endif // _EASYEDITOR_CAMERA_MGR_H_