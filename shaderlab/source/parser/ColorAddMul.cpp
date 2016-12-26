#include "ColorAddMul.h"
#include "Attribute.h"
#include "Varying.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

ColorAddMul::ColorAddMul()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "color"));
	m_attributes.push_back(new Attribute(VT_FLOAT4, "additive"));

	m_varyings.push_back(new Varying(VT_FLOAT4, "color"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "additive"));
}

std::string& ColorAddMul::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	std::string s = " \
		vec4 _col_add_multi_;\n \
		_col_add_multi_.xyz = _TMP_.xyz * v_color.xyz;\n \
		_col_add_multi_.w = _TMP_.w;\n \
		_col_add_multi_ *= v_color.w;\n \
		_col_add_multi_.xyz += v_additive.xyz * _TMP_.w * v_color.w;\n \
		_col_add_multi_.xyzw += v_additive.w * 10.0 * _col_add_multi_.xyzw;\n ";

	StringHelper::ReplaceAll(s, "_TMP_", m_input->GetOutput().GetName());
	str += s;
	return str;
}

Variable ColorAddMul::GetOutput() const
{
	return Variable(VT_FLOAT4, "_col_add_multi_");
}

}
}