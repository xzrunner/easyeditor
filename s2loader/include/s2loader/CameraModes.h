#ifndef _S2LOADER_CAMERA_MODES_H_
#define _S2LOADER_CAMERA_MODES_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <sprite2/CameraMode.h>

namespace s2loader
{

class CameraModes
{
public:
	s2::CameraMode Name2Mode(const CU_STR& name) const;
	CU_STR Mode2Name(s2::CameraMode mode) const;

private:
	struct Item
	{
		Item(s2::CameraMode mode, const CU_STR& name)
			: mode(mode), name(name) {}

		s2::CameraMode mode;
		CU_STR name;
	};

private:
	CU_VEC<Item> m_modes;

	CU_SINGLETON_DECLARATION(CameraModes);

}; // CameraModes

}

#endif // _S2LOADER_CAMERA_MODES_H_
