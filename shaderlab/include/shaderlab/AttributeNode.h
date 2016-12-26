#ifndef _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_
#define _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class AttributeNode : public Node
{
public:
	AttributeNode(Variable var);

	virtual std::string& ToStatements(std::string& str) const;

	virtual Variable GetOutput() const;

private:
	Variable m_var;


}; // AttributeNode

}
}

#endif // _SHADERLAB_PARSER_ATTRIBUTE_NODE_H_