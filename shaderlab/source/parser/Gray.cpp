#include "Gray.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

std::string Gray::GetBody() const 
{
	if (!m_input) {
		return "";
	}
	
	CheckType(m_input->GetOutput(), VT_FLOAT4);

	std::string s = "float _gray_ = dot(_TMP_.rgb , vec3(0.299, 0.587, 0.114));\n\
					 vec4 _DST_COL_ = vec4(_gray_, _gray_, _gray_, _TMP_.a);\n";
	StringHelper::ReplaceAll(s, "_TMP_", m_input->GetOutput().GetName());

	return s;
}

}
}