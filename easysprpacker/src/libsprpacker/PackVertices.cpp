#include "PackVertices.h"
#include "binary_io.h"
#include "to_int.h"

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <ee/Exception.h>

namespace esprpacker
{

void PackVertices::PackToLua(ebuilder::CodeGenerator& gen, const std::vector<sm::vec2>& vertices, 
							 const std::string& name)
{
	lua::assign_with_end(gen, name + "_num", vertices.size());

	std::stringstream ss;
	ss << name << " = {";
	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int16_t x = float16x2int(pos.x),
			    y = float16x2int(pos.y);
		ss << x << ", " << y << ", ";
	}
	ss << "}";
	gen.line(ss.str());
}

int PackVertices::SizeOfUnpackFromBin(const std::vector<sm::vec2>& vertices)
{
	return vertices.size() * sizeof(int16_t) * 2;
}

int PackVertices::SizeOfPackToBin(const std::vector<sm::vec2>& vertices)
{
	int sz = 0;
	sz += sizeof(uint16_t);								// number
	sz += sizeof(int16_t) * 2 * vertices.size();		// vertices
	return sz;
}

void PackVertices::PackToBin(const std::vector<sm::vec2>& vertices, uint8_t** ptr)
{
	if (vertices.size() > USHRT_MAX) {
		throw ee::Exception("PackVertices::PackToBin num too large.");
	}

	uint16_t num = vertices.size();
	pack(num, ptr);

	for (int i = 0, n = vertices.size(); i < n; ++i) 
	{
		const sm::vec2& pos = vertices[i];
		int16_t x = float16x2int(pos.x),
			    y = float16x2int(pos.y);
		pack(x, ptr);
		pack(y, ptr);
	}
}

}