#include "UnpackFromBin.h"
#include "UnpackNodeFactory.h"
#include "tools.h"
#include "IPackNode.h"

#include <epbin.h>

namespace libcoco
{

#define LZMA_PROPS_SIZE 5

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

void UnpackFromBin::Unpack(const std::string& filepath, 
						   const std::vector<d2d::Image*>& images)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	assert(!fin.fail());

	int32_t sz;
	unpack(sz, fin);

	if (sz < 0)
	{
		sz = -sz;
		uint8_t* buf = new uint8_t[sz];
		fin.read(reinterpret_cast<char*>(buf), sz);
		Unpack(&buf, images);
		delete[] buf;
	}
	else
	{
		uint8_t* c_buf = new uint8_t[sz];
		struct block* block = (struct block*)c_buf;
		fin.read(reinterpret_cast<char*>(block), sz);

		size_t uc_sz = sz * 100;		// FIXME
		size_t guess_sz = uc_sz;
		uint8_t* uc_buf = new uint8_t[uc_sz];
		size_t c_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
		epbin::Lzma::Uncompress(uc_buf, &uc_sz, block->data, &c_sz, block->prop, LZMA_PROPS_SIZE);
		if (guess_sz == uc_sz) {
			throw d2d::Exception("libcoco::UnpackFromBin::Unpack no enough space.");
		}
		delete[] c_buf;

		uint8_t* ptr = uc_buf;
		Unpack(&ptr, images);

		delete[] uc_buf;
	}

	UnpackNodeFactory::Instance()->AfterUnpack();
}

void UnpackFromBin::Unpack(uint8_t** ptr, const std::vector<d2d::Image*>& images)
{
	UnpackNodeFactory* factory = UnpackNodeFactory::Instance();

	uint16_t export_n;
	unpack(export_n, ptr);

	uint16_t maxid;
	unpack(maxid, ptr);

	uint16_t tex;
	unpack(tex, ptr);

	uint32_t unpack_sz;
	unpack(unpack_sz, ptr);

	uint32_t body_sz;
	unpack(body_sz, ptr);

	std::map<int, std::string> map_export;
	for (int i = 0; i < export_n; ++i) {
		uint16_t id;
		unpack(id, ptr);

		std::string name;
		unpack_str(name, ptr);

		map_export.insert(std::make_pair(id, name));
	}

	for (int i = 0; i < body_sz; ) {
		const IPackNode* node = factory->UnpackFromBin(ptr, images, map_export);
		i += node->SizeOfPackToBin();
	}
}

}