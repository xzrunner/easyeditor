#ifndef _SHADERLAB_PARSER_BLUR_VERT_H_
#define _SHADERLAB_PARSER_BLUR_VERT_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "gaussian_blur_vert.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    gaussian blur filter
 */
class GaussianBlurVert : public Filter
{
public:
	GaussianBlurVert() 
		: Filter("_col_gaussian_blur_") 
	{
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "tex_height"));
	}
	
	virtual std::string& GetHeader(std::string& str) const { 
		str += gaussian_blur_vert_header;
		return str; 
	}

protected:
	virtual std::string GetBody() const {
		return gaussian_blur_vert_body;
	}

}; // GaussianBlurVert

}
}

#endif // _SHADERLAB_PARSER_BLUR_VERT_H_