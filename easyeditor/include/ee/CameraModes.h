#ifndef _EASYEDITOR_CAMERA_MODES_H_
#define _EASYEDITOR_CAMERA_MODES_H_

#include <sprite2/CameraMode.h>

#include <string>
#include <vector>

namespace ee
{

class CameraModes
{
public:
	s2::CameraMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(s2::CameraMode mode) const;

	s2::CameraMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(s2::CameraMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	static CameraModes* Instance();

private:
	CameraModes();

private:
	struct Item
	{
		Item(s2::CameraMode mode, const std::string& name_en, const std::string& name_cn)
			: mode(mode), name_en(name_en), name_cn(name_cn) {}

		s2::CameraMode mode;
		std::string name_en;
		std::string name_cn;
	};

private:
	std::vector<Item> m_modes;

private:
	static CameraModes* m_instance;

}; // CameraModes

}

#endif // _EASYEDITOR_CAMERA_MODES_H_