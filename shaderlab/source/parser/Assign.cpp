#include "Assign.h"
#include "Variable.h"

#include <stdio.h>

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_col_static_";

Assign::Assign(Variable left, Variable right)
	: m_left(left)
	, m_right(right)
{
}

Assign::Assign(Variable left, float r, float g, float b, float a)
	: m_left(left)
	, m_right(VT_FLOAT4)
{
	char buf[256];
	sprintf(buf, "vec4(%f, %f, %f, %f)", r, g, b, a);
	m_right.SetName(buf);
}

std::string& Assign::ToStatements(std::string& str) const
{
	char buf[256];
	sprintf(buf, "%s %s = %s;\n", VAR_INFOS[m_left.GetType()].name, m_left.GetName(), m_right.GetName());
	str += buf;
	return str;
}

Variable Assign::GetOutput() const
{
	return m_left;
}

}
}