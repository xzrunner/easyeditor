#include "ChangedSectorIcon.h"

namespace eicon
{

ChangedSectorIcon::ChangedSectorIcon()
	: m_min(d2d::PI * 0.5f)
	, m_max(0)
{
}

void ChangedSectorIcon::Draw(const d2d::Matrix& mt, float process) const
{
	if (!m_img) {
		return;
	}

 	d2d::Vector region[4];
	GetTexcoords4(region, process);

	Icon::Draw(mt, region);
}

void ChangedSectorIcon::LoadFromFile(const Json::Value& value)
{
}

void ChangedSectorIcon::StoreToFile(Json::Value& value) const
{
}

void ChangedSectorIcon::GetRegion(float process, d2d::Rect& region) const
{
	if (!m_img) {
		return;
	}

	region = m_img->getRegion();
}

void ChangedSectorIcon::GetTexcoords4(d2d::Vector tex4[4], float process) const
{
	tex4[0].set(0, 0);
	tex4[1].set(0, 1);

	float angle = m_min + (m_max - m_min) * process;
	float tan_val = std::tan(angle);

	float w = m_img->GetClippedWidth(),
		h = m_img->GetClippedHeight();
	float x = h / tan_val;
	if (x < w) {
		tex4[2].set(x / w, 1.0f);
		tex4[3] = tex4[2];
	} else {
		float y = w * tan_val;
		assert(y <= h);
		tex4[2].set(1, 1);
		tex4[3].set(1, y / h);
	}
}

void ChangedSectorIcon::SetRegion(float angle_min, float angle_max)
{	
}

}