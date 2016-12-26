#ifndef _SHADERLAB_PARSER_RELIEF_H_
#define _SHADERLAB_PARSER_RELIEF_H_

#include "Filter.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "shock_wave.frag"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    shock wave
 *
 *  @remarks
 *    input: uniform float u_time;
 *           uniform vec2 u_center;
 *           uniform vec3 u_params;
 */
class ShockWave : public Filter
{
public:
	ShockWave() : Filter("_col_shock_wave_") {
		m_uniforms.push_back(new Uniform(VT_FLOAT1, "time"));
		m_uniforms.push_back(new Uniform(VT_FLOAT2, "center"));
		m_uniforms.push_back(new Uniform(VT_FLOAT3, "params"));
	}
	
protected:
	virtual std::string GetBody() const {
		return shock_wave_body;
	}

}; // ShockWave

}
}

#endif // _SHADERLAB_PARSER_RELIEF_H_