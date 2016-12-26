#ifndef _SHADERLAB_PARSER_VARYING_NODE_H_
#define _SHADERLAB_PARSER_VARYING_NODE_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class VaryingNode : public Node
{
public:
	VaryingNode(Variable var);

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

private:
	Variable m_var;

}; // VaryingNode

}
}

#endif // _SHADERLAB_PARSER_VARYING_NODE_H_