#ifndef _SHADERLAB_PARSER_VARIABLE_TYPE_H_
#define _SHADERLAB_PARSER_VARIABLE_TYPE_H_

namespace sl
{
namespace parser
{

enum VariableType
{
	VT_UNKNOWN = 0,

	VT_FLOAT1,
	VT_FLOAT2,
	VT_FLOAT3,
	VT_FLOAT4,

	VT_INT1,

	VT_MAT3,
	VT_MAT4,

	VT_SAMPLER2D,

	VT_MAX_COUNT,

}; // VariableType

struct VariableInfo
{
	const char* name;
//	int size;
};

static const VariableInfo VAR_INFOS[VT_MAX_COUNT] = 
{
// 	{ "unknown", 0 },
// 
// 	{ "float", 1 },
// 	{ "vec2", 2 },
// 	{ "vec3", 3 },
// 	{ "vec4", 4 },
// 
// 	{ "int", 1 },
// 
// 	{ "mat3", 9 },
// 	{ "mat4", 16 },
// 
// 	{ "sampler2D", 0 },

	//////////////////////////////////////////////////////////////////////////

	{ "unknown" },

	{ "float" },
	{ "vec2" },
	{ "vec3" },
	{ "vec4" },

	{ "int" },

	{ "mat3" },
	{ "mat4" },

	{ "sampler2D" },
};

static const int VT_SIZE[VT_MAX_COUNT] = 
{
	
};

}
}

#endif // _SHADERLAB_PARSER_VARIABLE_TYPE_H_