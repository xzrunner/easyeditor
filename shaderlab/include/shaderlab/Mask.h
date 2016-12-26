#ifndef _SHADERLAB_PARSER_MASK_H_
#define _SHADERLAB_PARSER_MASK_H_

#include "Node.h"

namespace sl
{
namespace parser
{

/**
 *  @brief
 *    layer mask
 */
class Mask : public Node
{
public:
	Mask();

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

}; // Mask

}
}

#endif // _SHADERLAB_PARSER_MASK_H_