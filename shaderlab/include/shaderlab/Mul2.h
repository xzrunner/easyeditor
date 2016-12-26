#ifndef _SHADERLAB_PARSER_MUL2_H_
#define _SHADERLAB_PARSER_MUL2_H_

#include "Node.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

class Mul2 : public Node
{
public:
	Mul2(Variable left, Variable right0, Variable right1) 
		: m_left(left), m_right0(right0), m_right1(right1) {}

	virtual std::string& ToStatements(std::string& str) const {
		CheckType(m_left, m_right0);
		CheckType(m_left, m_right1);
		char buf[128];
		sprintf(buf, "%s %s = %s * %s;\n", VAR_INFOS[m_left.GetType()].name, m_left.GetName(), m_right0.GetName(), m_right1.GetName());
		str += buf;
		return str;
	}

	virtual Variable GetOutput() const {
		return m_left;
	}

private:
	Variable m_left;
	Variable m_right0, m_right1;
	
}; // Mul2

}
}

#endif // _SHADERLAB_PARSER_MUL2_H_