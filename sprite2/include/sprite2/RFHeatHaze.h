#ifndef _SPRITE2_RF_HEAT_HAZE_H_
#define _SPRITE2_RF_HEAT_HAZE_H_

#include "RenderFilter.h"

#include <string>

namespace s2
{

class RFHeatHaze : public RenderFilter
{
public:
	RFHeatHaze() 
		: RenderFilter(FM_HEAT_HAZE) 
		, m_distortion_factor(0.02f)
		, m_rise_factor(0.2f)
	{}

	const std::string& GetFilepath() const { return m_filepath; }
	void SetFilepath(const std::string& filepath) { m_filepath = filepath; }

	void GetFactor(float& distortion, float& rise) const {
		distortion = m_distortion_factor;
		rise = m_rise_factor;
	}
	void SetFactor(float distortion, float rise) {
		m_distortion_factor = distortion;
		m_rise_factor = rise;
	}

private:
	std::string m_filepath;		// distortion map texture

	float m_distortion_factor, m_rise_factor;

}; // RFHeatHaze

}

#endif // _SPRITE2_RF_HEAT_HAZE_H_