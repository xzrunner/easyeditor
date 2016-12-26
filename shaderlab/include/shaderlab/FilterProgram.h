#ifndef _SHADERLAB_FILTER_PROGRAM_H_
#define _SHADERLAB_FILTER_PROGRAM_H_

#include "ShaderProgram.h"

namespace sl
{

class FilterProgram : public ShaderProgram
{
public:
	FilterProgram(ur::RenderContext* rc, int max_vertex);
	
	virtual void UpdateTime(float time) {}

protected:
	void Init(const std::vector<ur::VertexAttrib>& va_list, RenderBuffer* ib, 
		parser::Node* pn, parser::Node* pre_pn = NULL);

}; // FilterProgram

}

#endif // _SHADERLAB_FILTER_PROGRAM_H_