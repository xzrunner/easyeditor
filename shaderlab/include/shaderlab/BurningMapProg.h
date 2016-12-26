#ifndef _SHADERLAB_BURNING_MAP_PROG_H_
#define _SHADERLAB_BURNING_MAP_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class BurningMapProg : public FilterProgram
{
public:
	BurningMapProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	virtual void UpdateTime(float time);

	void SetLifeTime(float lifetime);

	void SetUpperTex(int tex);
	void SetHeightMapTex(int tex);
	void SetBorderGradientTex(int tex);

private:
	int m_lifetime, m_time;

	int m_upper_tex, m_height_map_tex, m_border_gradient_tex;

}; // BurningMapProg

}

#endif // _SHADERLAB_BURNING_MAP_PROG_H_