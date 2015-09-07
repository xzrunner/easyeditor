#include "LoadFromBin.h"
#include "NodeToSprite.h"
#include "Symbol.h"

#include <easycoco.h>
#include <epbin.h>

namespace ecomplex
{

void LoadFromBin::Load(const Json::Value& value, const std::string& dir, 
					   Symbol* symbol)
{
	std::string filename = d2d::FilenameTools::getAbsolutePath(dir, value["bin file"].asString());

	std::string ept_path = filename + ".ept";
	std::vector<d2d::Image*> images;
	LoadImages(ept_path, images);

	std::string epe_path = filename;
	libcoco::CocoUnpacker unpacker;
	unpacker.UnpackBin(epe_path, images);

	std::string export_name = value["export name"].asString();

	libcoco::IPackNode* node = libcoco::UnpackNodeFactory::Instance()->Query(export_name);
	symbol->m_sprites.push_back(NodeToSprite::ToSprite(node));
}

#define LZMA_PROPS_SIZE 5

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

void LoadFromBin::LoadImages(const std::string& filename, std::vector<d2d::Image*>& images)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	assert(!fin.fail());

	do {
		int32_t sz;
		libcoco::unpack(sz, fin);

		if (fin.fail()) {
			break;
		}

		if (sz < 0) 
		{
			sz = -sz;
			uint8_t* buf = new uint8_t[sz];
			fin.read(reinterpret_cast<char*>(buf), sz);
			LoadImage(&buf, images);
			delete[] buf;
		} 
		else 
		{
			uint8_t* c_buf = new uint8_t[sz];
			struct block* block = (struct block*)c_buf;
			fin.read(reinterpret_cast<char*>(block), sz);

			size_t uc_sz = sz * 10;		// FIXME
			uint8_t* uc_buf = new uint8_t[uc_sz];
			size_t c_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
			epbin::Lzma::Uncompress(uc_buf, &uc_sz, block->data, &c_sz, block->prop, LZMA_PROPS_SIZE);
			delete[] c_buf;

 			uint8_t* ptr = uc_buf;
			LoadImage(&ptr, images);

			delete[] uc_buf;
		}
	} while (!fin.fail());
}

void LoadFromBin::LoadImage(uint8_t** ptr, std::vector<d2d::Image*>& images)
{
	int8_t type;
	libcoco::unpack(type, ptr);

	int16_t w, h;
	libcoco::unpack(w, ptr);
	libcoco::unpack(h, ptr);

	const int c = 4;
	int buf_sz = w * h * c;
	uint8_t* buf = new uint8_t[buf_sz];
	memcpy(buf, *ptr, buf_sz);
	d2d::ImageData* data = new d2d::ImageData(buf, w, h, c);
	images.push_back(new d2d::Image(data));
}

}