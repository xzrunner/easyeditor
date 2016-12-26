#ifndef _SHADERLAB_PARSER_GOURAUD_SHADING_H_
#define _SHADERLAB_PARSER_GOURAUD_SHADING_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    Gouraud shading
 *
 *  @remarks
 *    input: attribute vec3 normal;
 *           uniform vec3 u_diffuse_material;
 *           uniform vec3 u_ambient_material;
 *           uniform vec3 u_specular_material;
 *           uniform float u_shininess;
 *           varying vec4 v_gouraud_col;
 *    
 *    output: gl_Position
 */
class GouraudShading : public Node
{
public:
	GouraudShading();

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

	const char* GetSpecularName() const;

}; // GouraudShading

}
}

#endif // _SHADERLAB_PARSER_GOURAUD_SHADING_H_