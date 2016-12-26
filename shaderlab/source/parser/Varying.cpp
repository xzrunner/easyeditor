#include "Varying.h"

namespace sl
{
namespace parser
{

Varying::Varying(const Variable& var)
	: Variable(var)
{
}

Varying::Varying(VariableType type, std::string name)
	: Variable(type, name)
{
}

std::string& Varying::ToStatement(std::string& str) const
{
	str += "varying ";
	str += VAR_INFOS[m_type].name;
	str += " v_" + m_name + ";\n";
	return str;
}

}
}