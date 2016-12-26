#ifndef _SHADERLAB_PARSER_BLUR_H_
#define _SHADERLAB_PARSER_BLUR_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "blur.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    simple blur
 *
 *  @remarks
 *    input: uniform float u_radius
 */
class Blur : public Filter
{
public:
	Blur() : Filter("_col_blur_") {
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "radius"));
	}
	
protected:
	virtual std::string GetBody() const {
		return blur_body;
	}

}; // Blur

}
}

#endif // _SHADERLAB_PARSER_BLUR_H_