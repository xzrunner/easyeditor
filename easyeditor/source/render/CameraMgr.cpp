#include "CameraMgr.h"
#include "OrthoCamera.h"
#include "Pseudo3DCamera.h"

namespace ee
{

CameraMgr* CameraMgr::m_instance = NULL;

CameraMgr* CameraMgr::Instance()
{
	if (!m_instance) {
		m_instance = new CameraMgr;
	}
	return m_instance;
}

CameraMgr::CameraMgr()
	: m_curr_type(ORTHO)
	, m_curr(new OrthoCamera)
{
}

CameraMgr::~CameraMgr()
{
	delete m_curr;
}

void CameraMgr::SetCamera(Type t)
{
	if (m_curr_type == t) {
		return;
	}	
	
	delete m_curr;
	switch (t)
	{
	case ORTHO:
		m_curr = new OrthoCamera();
		break;
	case PSEUDO3D:
		m_curr = new Pseudo3DCamera();
		break;
	}
	m_curr_type = t;
}

Camera* CameraMgr::GetCamera() const
{
	return m_curr;
}

bool CameraMgr::IsType(Type t) const
{
	return m_curr_type == t;
}

}