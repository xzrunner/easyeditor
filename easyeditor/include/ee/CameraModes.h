#ifndef _EASYEDITOR_CAMERA_MODES_H_
#define _EASYEDITOR_CAMERA_MODES_H_

#include <cu/cu_macro.h>

#include <painting2/CameraMode.h>

#include <string>
#include <vector>

namespace ee
{

class CameraModes
{
public:
	pt2::CameraMode ID2Mode(int idx) const;
	int Mode2ID(pt2::CameraMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

private:
	struct Item
	{
		Item(pt2::CameraMode mode, const std::string& name)
			: mode(mode), name(name) {}

		pt2::CameraMode mode;
		std::string name;
	};

private:
	std::vector<Item> m_modes;

	CU_SINGLETON_DECLARATION(CameraModes);

}; // CameraModes

}

#endif // _EASYEDITOR_CAMERA_MODES_H_