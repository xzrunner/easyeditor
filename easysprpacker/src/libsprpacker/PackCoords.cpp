#include "PackCoords.h"
#include "binary_io.h"
#include "to_int.h"

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <ee/Exception.h>

namespace esprpacker
{

void PackCoords::PackToLua(ebuilder::CodeGenerator& gen, const std::vector<sm::vec2>& vertices, 
						   const std::string& name, int precision)
{
	lua::assign_with_end(gen, name + "_num", vertices.size());

	std::stringstream ss;
	ss << name << " = {";
	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int16_t x = float2int(pos.x, precision),
			    y = float2int(pos.y, precision);
		ss << x << ", " << y << ", ";
	}
	ss << "}";
	gen.line(ss.str());
}

int PackCoords::SizeOfUnpackFromBin(const std::vector<sm::vec2>& vertices)
{
	return vertices.size() * sizeof(int16_t) * 2;
}

int PackCoords::SizeOfPackToBin(const std::vector<sm::vec2>& vertices)
{
	int sz = 0;
	sz += sizeof(uint16_t);
	sz += sizeof(int16_t) * 2 * vertices.size();
	return sz;
}

void PackCoords::PackToBin(const std::vector<sm::vec2>& vertices, uint8_t** ptr, int precision)
{
	if (vertices.size() > USHRT_MAX) {
		throw ee::Exception("PackCoords::PackToBin num too large.");
	}

	uint16_t num = vertices.size();
	pack(num, ptr);

	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int16_t x = float2int(pos.x, precision),
			    y = float2int(pos.y, precision);
		pack(x, ptr);
		pack(y, ptr);
	}
}

void PackCoords::PackToLuaF32(ebuilder::CodeGenerator& gen, const std::vector<sm::vec2>& vertices, const std::string& name)
{
	lua::assign_with_end(gen, name + "_num", vertices.size());

	std::stringstream ss;
	ss << name << " = {";
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ss << vertices[i].x << ", " << vertices[i].y << ", ";
	}
	ss << "}";
	gen.line(ss.str());
}

int PackCoords::SizeOfUnpackFromBinF32(const std::vector<sm::vec2>& vertices)
{
	return vertices.size() * sizeof(float) * 2;
}

int PackCoords::SizeOfPackToBinF32(const std::vector<sm::vec2>& vertices)
{
	int sz = 0;
	sz += sizeof(uint16_t);
	sz += sizeof(float) * 2 * vertices.size();
	return sz;
}

void PackCoords::PackToBinF32(const std::vector<sm::vec2>& vertices, uint8_t** ptr)
{
	if (vertices.size() > USHRT_MAX) {
		throw ee::Exception("PackCoords::PackToBin num too large.");
	}

	uint16_t num = vertices.size();
	pack(num, ptr);

	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		float x = pos.x,
			  y = pos.y;
		pack(x, ptr);
		pack(y, ptr);
	}
}

}