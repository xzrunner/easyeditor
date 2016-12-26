#ifndef _SHADERLAB_PARSER_POSITION_TRANS_H_
#define _SHADERLAB_PARSER_POSITION_TRANS_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    vertex position
 *
 *  @remarks
 *    input: attribute vec4 position;
 *           uniform mat4 u_projection;
 *           uniform mat4 u_modelview;
 *    
 *    output: gl_Position
 */
class PositionTrans : public Node
{
public:
	PositionTrans();

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const { return Variable(VT_FLOAT4, "gl_Position"); }

}; // PositionTrans

}
}

#endif // _SHADERLAB_PARSER_POSITION_TRANS_H_