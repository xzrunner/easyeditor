#include "PositionTrans.h"
#include "Attribute.h"
#include "ParserUniform.h"

namespace sl
{
namespace parser
{

PositionTrans::PositionTrans()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "position"));

	m_uniforms.push_back(new Uniform(VT_MAT4, "projection"));
	m_uniforms.push_back(new Uniform(VT_MAT4, "modelview"));
}

std::string& PositionTrans::ToStatements(std::string& str) const
{
	str += "gl_Position = u_projection * u_modelview * position;\n";
	return str;
}

}
}