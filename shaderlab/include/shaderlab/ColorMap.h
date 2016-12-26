#ifndef _SHADERLAB_PARSER_COLOR_MAP_H_
#define _SHADERLAB_PARSER_COLOR_MAP_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    transform each r, g, b
 *
 *  @remarks
 *    input: varying vec4 v_rmap;
 *           varying vec4 v_gmap;
 *           varying vec4 v_bmap;
 *    
 *    output: gl_Position
 */
class ColorMap : public Node
{
public:
	ColorMap();

	virtual std::string& GetHeader(std::string& str) const;
	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const;

}; // ColorMap

}
}

#endif // _SHADERLAB_PARSER_COLOR_MAP_H_