#include "CameraMgr.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"

namespace ee
{

SINGLETON_DEFINITION(CameraMgr)

CameraMgr::CameraMgr()
	: m_curr_cam(MAX_COUNT)
{
	memset(m_cameras, 0, sizeof(m_cameras));
	SetCamera(ORTHO);
}

CameraMgr::~CameraMgr()
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		delete m_cameras[i];
	}
}

void CameraMgr::SetCamera(Type t)
{
	if (t < ORTHO || t >= MAX_COUNT) {
		return;
	}
	m_curr_cam = t;
	if (m_cameras[m_curr_cam]) {
		return;
	}

	switch (m_curr_cam)
	{
	case ORTHO:
		m_cameras[ORTHO] = new OrthoCamera();
		break;
	case PSEUDO3D:
		m_cameras[PSEUDO3D] = new Pseudo3DCamera();
		break;
	}
}

Camera* CameraMgr::GetCamera() const
{
	return m_cameras[m_curr_cam];
}

Camera* CameraMgr::GetCamera(Type t) const
{
	if (t >= ORTHO && t < MAX_COUNT) {
		return m_cameras[t];
	} else {
		return NULL;
	}
}

bool CameraMgr::IsType(Type t) const
{
	return m_curr_cam == t;
}

}