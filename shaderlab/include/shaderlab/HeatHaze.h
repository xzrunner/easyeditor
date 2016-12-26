#ifndef _SHADERLAB_PARSER_HEAT_HAZE_H_
#define _SHADERLAB_PARSER_HEAT_HAZE_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "heat_haze.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    heat haze
 *
 *  @remarks
 *    input: uniform sampler2D u_current_tex;			// Our render texture
 *           uniform sampler2D u_distortion_map_tex;	// Our heat distortion map texture
 *           uniform float u_time;						// Time used to scroll the distortion map
 *           uniform float u_distortion_factor;			// Factor used to control severity of the effect
 *           uniform float u_rise_factor;				// Factor used to control how fast air rises
 */
class HeatHaze : public Filter
{
public:
	HeatHaze() : Filter("_col_heat_haze_") {
		m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "distortion_map_tex"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "time"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "distortion_factor"));
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "rise_factor"));
	}
	
protected:
	virtual std::string GetBody() const {
		return heat_haze_body;
	}

}; // HeatHaze

}
}

#endif // _SHADERLAB_PARSER_HEAT_HAZE_H_