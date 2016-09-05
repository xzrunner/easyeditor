#ifndef _EASYEDITOR_CAMERA_MODES_H_
#define _EASYEDITOR_CAMERA_MODES_H_

#include <CU_Singleton.h>

#include <sprite2/CameraMode.h>

#include <string>
#include <vector>

namespace ee
{

class CameraModes
{
public:
	s2::CameraMode ID2Mode(int idx) const;
	int Mode2ID(s2::CameraMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

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

#endif // _EASYEDITOR_CAMERA_MODES_H_