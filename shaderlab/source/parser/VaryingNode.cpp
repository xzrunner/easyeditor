#include "VaryingNode.h"
#include "Varying.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

VaryingNode::VaryingNode(Variable var)
	: m_var(var)
{
	m_varyings.push_back(new Varying(var));
}

std::string& VaryingNode::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_var, m_input->GetOutput());

	char buf[128];
	sprintf(buf, "v_%s = %s;\n", m_var.GetName(), m_input->GetOutput().GetName());
	str += buf;
	return str;
}

Variable VaryingNode::GetOutput() const
{
	return Variable(m_var.GetType(), std::string("v_") + m_var.GetName());
}

}
}