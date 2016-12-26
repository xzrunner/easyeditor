#ifndef _SHADERLAB_SHOCK_WAVE_PROG_H_
#define _SHADERLAB_SHOCK_WAVE_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class ShockWaveProg : public FilterProgram
{
public:
	ShockWaveProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	virtual void UpdateTime(float time);

	void SetCenter(float center[2]);

	void SetFactor(float params[3]);

private:
	int m_time;

	int m_center, m_params;

}; // ShockWaveProg

}

#endif // _SHADERLAB_SHOCK_WAVE_PROG_H_