#include "ColorMap.h"
#include "Attribute.h"
#include "Varying.h"
#include "StringHelper.h"

namespace sl
{
namespace parser
{

static const char* OUTPUT_NAME = "_col_map_";

ColorMap::ColorMap()
{
	m_attributes.push_back(new Attribute(VT_FLOAT4, "rmap"));
	m_attributes.push_back(new Attribute(VT_FLOAT4, "gmap"));
	m_attributes.push_back(new Attribute(VT_FLOAT4, "bmap"));

	m_varyings.push_back(new Varying(VT_FLOAT4, "rmap"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "gmap"));
	m_varyings.push_back(new Varying(VT_FLOAT4, "bmap"));
}

std::string& ColorMap::GetHeader(std::string& str) const
{
	str += "\n#define neql(a, b) (step(0.0001, abs((a) - (b))))\n";
	return str;
}

std::string& ColorMap::ToStatements(std::string& str) const
{
	if (!m_input) {
		return str;
	}

	CheckType(m_input->GetOutput(), VT_FLOAT4);

	std::string s = " \
		float s = 1.2;\n \
		float k = _TMP_.r + _TMP_.g + _TMP_.b;\n \
		\
		float r_valid = step(0.5, neql(v_rmap.r, 1.0) + neql(v_rmap.g, 0.0) + neql(v_rmap.b, 0.0));\n \
		float cmp_gr = step(_TMP_.g * s, _TMP_.r);\n \
		float cmp_br = step(_TMP_.b * s, _TMP_.r);\n \
		vec3 dr = (v_rmap.rgb * k - _TMP_.rgb) * r_valid * cmp_gr * cmp_br;\n \
		\
		float g_valid = step(0.5, neql(v_gmap.r, 0.0) + neql(v_gmap.g, 1.0) + neql(v_gmap.b, 0.0));\n	\
		float cmp_rg = step(_TMP_.r * s, _TMP_.g);\n \
		float cmp_bg = step(_TMP_.b * s, _TMP_.g);\n \
		vec3 dg = (v_gmap.rgb * k - _TMP_.rgb) * g_valid * cmp_rg * cmp_bg;\n \
		\
		float b_valid = step(0.5, neql(v_bmap.r, 0.0) + neql(v_bmap.g, 0.0) + neql(v_bmap.b, 1.0));\n	\
		float cmp_rb = step(_TMP_.r * s, _TMP_.b);\n \
		float cmp_gb = step(_TMP_.g * s, _TMP_.b);\n \
		vec3 db = (v_bmap.rgb * k - _TMP_.rgb) * b_valid * cmp_rb * cmp_gb;\n \
		\
		vec4 _col_map_ = vec4(dr + dg + db + _TMP_.rgb, _TMP_.a);\n \
		";
	StringHelper::ReplaceAll(s, "_TMP_", m_input->GetOutput().GetName());
	str += s;
	return str;
}

Variable ColorMap::GetOutput() const
{
	return Variable(VT_FLOAT4, OUTPUT_NAME);
}

}
}