#ifndef _EASYEDITOR_CAMERA_MODES_H_
#define _EASYEDITOR_CAMERA_MODES_H_

#include <string>
#include <vector>

namespace ee
{

enum CameraMode
{
	CM_ORTHO					= 0,

	CM_PERSPECTIVE_NO_HEIGHT	=  10,
	CM_PERSPECTIVE_AUTO_HEIGHT,
};

class CameraModes
{
public:
	CameraMode GetModeFromNameEN(const std::string& name) const;
	std::string GetNameENFromMode(CameraMode mode) const;

	CameraMode GetIDFromIdx(int idx) const;
	int GetIdxFromID(CameraMode mode) const;

	void GetAllNameCN(std::vector<std::string>& names) const;

	static CameraModes* Instance();

private:
	CameraModes();

private:
	struct Item
	{
		Item(CameraMode mode, const std::string& name_en, const std::string& name_cn)
			: mode(mode), name_en(name_en), name_cn(name_cn) {}

		CameraMode mode;
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