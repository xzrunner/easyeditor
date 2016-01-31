#include "LoadFromBin.h"
#include "NodeToSprite.h"
#include "Symbol.h"

#include <easyrespacker.h>

namespace ecomplex
{

void LoadFromBin::Load(const Json::Value& value, const std::string& dir, 
					   Symbol* symbol)
{
	std::string filename = ee::FileHelper::GetAbsolutePath(dir, value["bin file"].asString());

	std::string ept_path = filename;
	std::vector<ee::Image*> images;
	LoadImages(ept_path, images);

	std::string epe_path = filename + ".epe";
	librespacker::ResUnpacker unpacker;
	unpacker.UnpackBin(epe_path, images);

	std::string export_name = value["export name"].asString();

	librespacker::IPackNode* node = librespacker::UnpackNodeFactory::Instance()->Query(export_name);
	symbol->m_sprites.push_back(NodeToSprite::Trans(node));
}

#define LZMA_PROPS_SIZE 5

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

void LoadFromBin::LoadImages(const std::string& filepath, std::vector<ee::Image*>& images)
{
	int idx = 1;
	while (true) 
	{
		std::string _filepath = filepath + "." + ee::StringHelper::ToString(idx++) + ".ept";
		if (!ee::FileHelper::IsFileExist(_filepath)) {
			break;
		}

		std::locale::global(std::locale(""));
		std::ifstream fin(_filepath.c_str(), std::ios::binary);
		std::locale::global(std::locale("C"));
		assert(!fin.fail());

		int32_t sz;
		librespacker::unpack(sz, fin);

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
			size_t guess_sz = uc_sz;
			uint8_t* uc_buf = new uint8_t[uc_sz];
			size_t c_sz = sz - sizeof(block->size) - LZMA_PROPS_SIZE;
			librespacker::Lzma::Uncompress(uc_buf, &uc_sz, block->data, &c_sz, block->prop, LZMA_PROPS_SIZE);
			if (guess_sz == uc_sz) {
				throw ee::Exception("ecomplex LoadFromBin::LoadImages no enough space.");
			}
			delete[] c_buf;

			uint8_t* ptr = uc_buf;
			LoadImage(&ptr, images);

			delete[] uc_buf;
		}
	}
}

void LoadFromBin::LoadImage(uint8_t** ptr, std::vector<ee::Image*>& images)
{
	int8_t type;
	librespacker::unpack(type, ptr);

	int16_t w, h;
	librespacker::unpack(w, ptr);
	librespacker::unpack(h, ptr);

	const int c = 4;
	int buf_sz = w * h * c;
	uint8_t* buf = new uint8_t[buf_sz];
	memcpy(buf, *ptr, buf_sz);
	ee::ImageData* data = new ee::ImageData(buf, w, h, c);
	images.push_back(new ee::Image(data));
}

}