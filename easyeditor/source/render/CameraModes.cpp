#include "CameraModes.h"

#include <string>

namespace ee
{

SINGLETON_DEFINITION(CameraModes);

CameraModes::CameraModes()
{
	m_modes.push_back(Item(s2::CM_ORTHO, "正交"));
	m_modes.push_back(Item(s2::CM_PERSPECTIVE_NO_HEIGHT, "投影没高度"));
	m_modes.push_back(Item(s2::CM_PERSPECTIVE_AUTO_HEIGHT, "投影计算高度"));
}

s2::CameraMode CameraModes::ID2Mode(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::CM_ORTHO;
	} else {
		return m_modes[idx].mode;
	}
}

int CameraModes::Mode2ID(s2::CameraMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void CameraModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name);
	}
}

}