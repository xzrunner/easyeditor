#ifndef _GUM_CAMERA_MODES_H_
#define _GUM_CAMERA_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/CameraMode.h>

#include <string>
#include <vector>

namespace gum
{

class CameraModes
{
public:
	s2::CameraMode Name2Mode(const std::string& name) const;
	std::string Mode2Name(s2::CameraMode mode) const;

private:
	struct Item
	{
		Item(s2::CameraMode mode, const std::string& name)
			: mode(mode), name(name) {}

		s2::CameraMode mode;
		std::string name;
	};

private:
	std::vector<Item> m_modes;

	SINGLETON_DECLARATION(CameraModes);

}; // CameraModes

}

#endif // _GUM_CAMERA_MODES_H_
