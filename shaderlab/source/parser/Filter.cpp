#include "Filter.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

std::string& Filter::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	std::string s = GetBody();
	StringHelper::ReplaceAll(s, "_DST_COL_", m_output);
	str += s;
	return str;
}

}
}