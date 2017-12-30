#include "LoadFromEP.h"
#include "NodeToSprite.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/ImageData.h>
#include <ee/Image.h>
#include <ee/Sprite.h>

#include <easyrespacker.h>

#include <gimg_typedef.h>

namespace ecomplex
{

void LoadFromEP::Load(const Json::Value& value, const std::string& dir, Symbol& sym)
{
	std::string filename = ee::FileHelper::GetAbsolutePath(dir, value["bin file"].asString());

	std::string ept_path = filename;
	std::vector<ee::ImagePtr> images;
	LoadImages(ept_path, images);

	std::string epe_path = filename + ".epe";
	erespacker::ResUnpacker unpacker;
	unpacker.UnpackBin(epe_path, images);

	std::string export_name = value["export name"].asString();

	erespacker::IPackNode* node = erespacker::UnpackNodeFactory::Instance()->Query(export_name);
	sym.Add(NodeToSprite::Trans(node));
}

#define LZMA_PROPS_SIZE 5

struct block {
	uint8_t size[4];
	uint8_t prop[LZMA_PROPS_SIZE];
	uint8_t data[119];
};

void LoadFromEP::LoadImages(const std::string& filepath, std::vector<ee::ImagePtr>& images)
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
		erespacker::unpack(sz, fin);

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
			erespacker::Lzma::Uncompress(uc_buf, &uc_sz, block->data, &c_sz, block->prop, LZMA_PROPS_SIZE);
			if (guess_sz == uc_sz) {
				throw ee::Exception("ecomplex LoadFromEP::LoadImages no enough space.");
			}
			delete[] c_buf;

			uint8_t* ptr = uc_buf;
			LoadImage(&ptr, images);

			delete[] uc_buf;
		}
	}
}

void LoadFromEP::LoadImage(uint8_t** ptr, std::vector<ee::ImagePtr>& images)
{
	int8_t type;
	erespacker::unpack(type, ptr);

	int16_t w, h;
	erespacker::unpack(w, ptr);
	erespacker::unpack(h, ptr);

	int buf_sz = w * h * 4;
	uint8_t* buf = new uint8_t[buf_sz];
	memcpy(buf, *ptr, buf_sz);
	images.push_back(std::make_shared<ee::Image>(buf, w, h, GPF_RGBA8));
}

}