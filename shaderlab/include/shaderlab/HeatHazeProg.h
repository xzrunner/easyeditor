#ifndef _SHADERLAB_HEAT_HAZE_PROG_H_
#define _SHADERLAB_HEAT_HAZE_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class HeatHazeProg : public FilterProgram
{
public:
	HeatHazeProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	virtual void UpdateTime(float time);

	void SetFactor(float distortion, float rise);

	void SetDistortionMapTex(int tex);

private:
	int m_time;
	int m_distortion_factor, m_rise_factor;

	int m_distortion_map_tex;

}; // HeatHazeProg

}

#endif // _SHADERLAB_HEAT_HAZE_PROG_H_