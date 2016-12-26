#ifndef _SHADERLAB_PARSER_BLEND_H_
#define _SHADERLAB_PARSER_BLEND_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    layer blend
 */
class Blend : public Node
{
public:
	Blend();

	virtual std::string& GetHeader(std::string& str) const;
	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const;

}; // Blend

}
}

#endif // _SHADERLAB_PARSER_BLEND_H_