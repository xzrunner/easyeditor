#ifndef _SHADERLAB_COL_GRADING_PROG_H_
#define _SHADERLAB_COL_GRADING_PROG_H_

#include "FilterProgram.h"

namespace sl
{

class ColGradingProg : public FilterProgram
{
public:
	ColGradingProg(ur::RenderContext* rc, int max_vertex, 
		const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib);

	virtual void Bind();

	void SetLUTTex(int tex);

	bool IsTexValid() const { return m_lut_tex != 0; }

private:
	int m_lut_tex;

}; // ColGradingProg

}

#endif // _SHADERLAB_COL_GRADING_PROG_H_