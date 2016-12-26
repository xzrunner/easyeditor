#include "Blend.h"
#include "StringHelper.h"
#include "ParserUniform.h"

#define STRINGIFY(A)  #A
#include "blend.frag"

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_blend_dst_";

Blend::Blend()
{
	m_uniforms.push_back(new Uniform(VT_SAMPLER2D, "texture1"));
	m_uniforms.push_back(new Uniform(VT_INT1, "mode"));
}

std::string& Blend::GetHeader(std::string& str) const
{
	str += blend_header;
	return str;
}

std::string& Blend::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	std::string s = blend_body;
	StringHelper::ReplaceAll(s, "_SRC_COL_", m_input->GetOutput().GetName());
	StringHelper::ReplaceAll(s, "_DST_COL_", OUTPUT_NAME);
	str += s;
	return str;
}

Variable Blend::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME); 
}

}
}