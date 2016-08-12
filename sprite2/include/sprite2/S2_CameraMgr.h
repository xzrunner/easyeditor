#ifndef _SPRITE2_CAMERA_MGR_H_
#define _SPRITE2_CAMERA_MGR_H_

#include <CU_Singleton.h>

namespace s2
{

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
	bool IsType(Type t) const { return m_type == t; }

private:
	Type m_type;

	SINGLETON_DECLARATION(CameraMgr)

}; // CameraMgr

}

#endif // _SPRITE2_CAMERA_MGR_H_