#ifndef _SHADERLAB_PARSER_COLOR_GRADING_H_
#define _SHADERLAB_PARSER_COLOR_GRADING_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "color_grading.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    color grading
 *
 *  @remarks
 *    input: uniform sampler2D u_col_lut;			// LUT texture for color grading
 */
class ColorGrading : public Filter
{
public:
	ColorGrading() : Filter("_col_grading_") {
		m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "col_lut"));
	}
	
	virtual std::string& GetHeader(std::string& str) const {
		str += color_grading_header;
		return str;
	}

protected:
	virtual std::string GetBody() const {
		return color_grading_body;
	}

}; // ColorGrading

}
}

#endif // _SHADERLAB_PARSER_COLOR_GRADING_H_