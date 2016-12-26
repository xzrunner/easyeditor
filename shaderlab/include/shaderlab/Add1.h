#ifndef _SHADERLAB_PARSER_ADD1_H_
#define _SHADERLAB_PARSER_ADD1_H_

#include "Node.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

class Add1 : public Node
{
public:
	Add1(Variable left, Variable right) 
		: m_left(left), m_right(right) {}

	virtual std::string& ToStatements(std::string& str) const {
		CheckType(m_left, m_right);
		char buf[128];
		sprintf(buf, "%s += %s;\n", m_left.GetName(), m_right.GetName());
		str += buf;
		return str;
	}

	virtual Variable GetOutput() const {
		return m_left;
	}

private:
	Variable m_left, m_right;
	
}; // Add1

}
}

#endif // _SHADERLAB_PARSER_ADD1_H_