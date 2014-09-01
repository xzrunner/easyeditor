#include "FpsStat.h"

#include "common/color_config.h"
#include "common/Vector.h"
#include "render/LabelNew.h"
#include "view/Screen.h"

#include <sstream>

namespace d2d
{
	
FpsStat::FpsStat(float interval)
{	
	m_interval = interval * CLOCKS_PER_SEC;
	m_last_time = 0;
	m_tot_cost = m_count = 0;
}

void FpsStat::Begin() 
{
	m_last_time = clock();
}

void FpsStat::End() 
{
	if (m_tot_cost > m_interval || m_count > 60) {
		if (m_count != 0)
		{
			float dt = (float)m_tot_cost / m_count;	
			std::stringstream ss;
			ss << dt;
			m_time = "cost: "+ss.str()+" ms";
		}
		if (m_tot_cost != 0)
		{
			int fps = m_count * CLOCKS_PER_SEC / m_tot_cost;	
			std::stringstream ss;
			ss << fps;
			m_fps = "fps: "+ss.str();
		}
		m_tot_cost = m_count = 0;
	}

	clock_t curr = clock();
	m_tot_cost += (curr - m_last_time);
	++m_count;
}

void FpsStat::DrawTime(const Screen& scr) const
{
	Draw(scr, m_time);
}

void FpsStat::DrawFPS(const Screen& scr) const
{
	Draw(scr, m_fps);
}

void FpsStat::Draw(const Screen& scr, const std::string& str) const
{
	LabelStyle style;
	style.has_edge = false;
	style.font_size = 20;
	style.width = 100;
	style.height = 50;
	style.color = LIGHT_RED;
	style.align_hori = HAT_LEFT;
	style.align_vert = VAT_TOP;

	const Vector& size = scr.GetSize();
	Vector pos;
	pos.x = -size.x * 0.4f;
	pos.y = size.y * 0.45f;
	LabelNew::Print(Screen(size.x, size.y), m_time.c_str(), pos, style);
}

}