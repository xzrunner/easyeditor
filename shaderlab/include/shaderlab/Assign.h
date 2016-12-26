#ifndef _SHADERLAB_PARSER_ASSIGN_H_
#define _SHADERLAB_PARSER_ASSIGN_H_

#include "Node.h"

namespace sl
{
namespace parser
{

class Assign : public Node
{
public:
	Assign(Variable left, Variable right);
	Assign(Variable left, float r, float g, float b, float a);

	virtual std::string& ToStatements(std::string& str) const;
	
	virtual Variable GetOutput() const;

private:
	Variable m_left, m_right;

}; // Assign

}
}

#endif // _SHADERLAB_PARSER_ASSIGN_H_