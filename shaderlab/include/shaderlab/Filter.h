#ifndef _SHADERLAB_PARSER_FILTER_H_
#define _SHADERLAB_PARSER_FILTER_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    frag filter
 *
 *  @remarks
 *    type:    frag
 *    input:   varying vec2 v_texcoord
 *             uniform sampler2D texture0
 *    output:  vec4 color
 *    prev: TextureMap
 */
class Filter : public Node
{
public:
	Filter(const std::string& output) : m_output(output) {}
	
	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const {
		return Variable(VT_FLOAT4, m_output);
	}

protected:
	virtual std::string GetBody() const = 0;

private:
	std::string m_output;

}; // Filter

}
}

#endif // _SHADERLAB_PARSER_FILTER_H_