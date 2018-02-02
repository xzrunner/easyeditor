#include "s2loader/CameraModes.h"

namespace s2loader
{

CU_SINGLETON_DEFINITION(CameraModes);

CameraModes::CameraModes()
{
	m_modes.push_back(Item(pt2::CM_ORTHO, "ortho"));
	m_modes.push_back(Item(pt2::CM_PERSPECTIVE_NO_HEIGHT, "perspective no height"));
	m_modes.push_back(Item(pt2::CM_PERSPECTIVE_AUTO_HEIGHT, "perspective auto height"));
}

pt2::CameraMode CameraModes::Name2Mode(const CU_STR& name) const
{
	if (name.empty()) {
		return pt2::CM_ORTHO;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.mode;
		}
	}
	return pt2::CM_ORTHO;
}

CU_STR CameraModes::Mode2Name(pt2::CameraMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.mode == mode) {
			return item.name;
		}
	}
	return "";
}

}