#include "CameraModes.h"

#include <string>

namespace glue
{

SINGLETON_DEFINITION(CameraModes);

CameraModes::CameraModes()
{
	m_modes.push_back(Item(s2::CM_ORTHO, "ortho"));
	m_modes.push_back(Item(s2::CM_PERSPECTIVE_NO_HEIGHT, "perspective no height"));
	m_modes.push_back(Item(s2::CM_PERSPECTIVE_AUTO_HEIGHT, "perspective auto height"));
}

s2::CameraMode CameraModes::Name2Mode(const std::string& name) const
{
	if (name.empty()) {
		return s2::CM_ORTHO;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name == name) {
			return item.mode;
		}
	}
	return s2::CM_ORTHO;
}

std::string CameraModes::Mode2Name(s2::CameraMode mode) const
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