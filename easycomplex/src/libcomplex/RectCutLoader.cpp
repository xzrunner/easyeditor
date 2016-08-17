#include "RectCutLoader.h"

#include <ee/Symbol.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/sprite_msg.h>
#include <ee/FileHelper.h>
#include <ee/ImageClip.h>
#include <ee/ImageData.h>
#include <ee/Image.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageSprite.h>
#include <ee/Math2D.h>

#include <easyimage.h>

#include <json/json.h>

#include <fstream>

namespace ecomplex
{

void RectCutLoader::LoadOnlyJson(const std::string& pack_file, const std::string& img_name)
{
	std::vector<Picture> pictures;
	LoadJsonFile(pack_file, img_name, pictures);

	for (int i = 0, n = pictures.size(); i < n; ++i)
	{
		const Picture& s = pictures[i];
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(s.filepath);
		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);

		sm::vec2 pos;
		pos.x = s.src.x + s.src.w * 0.5f;
		pos.y = s.src.y + s.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		spr->SetPosition(pos);

		ee::InsertSpriteSJ::Instance()->Insert(spr);

		spr->RemoveReference();
		sym->RemoveReference();
	}
}

void RectCutLoader::LoadJsonAndImg(const std::string& pack_file, const std::string& img_name)
{
	std::vector<Picture> pictures;
	LoadJsonFile(pack_file, img_name, pictures);

	std::string dir = ee::FileHelper::GetFileDir(pack_file);
	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(dir + "\\pack.png");

	ee::ImageClip clip(*img);
	for (int i = 0, n = pictures.size(); i < n; ++i)
	{		
		const Picture& pic = pictures[i];
		const uint8_t* pixels = clip.Clip(pic.dst.x, pic.dst.x + pic.dst.w, 1024 - pic.dst.y - pic.dst.h, 1024 - pic.dst.y);

		ee::ImageData* img_data = new ee::ImageData(pixels, pic.dst.w, pic.dst.h, 4);
		ee::Image* spr_img = new ee::Image(img_data);
		ee::ImageSymbol* spr_symbol = new ee::ImageSymbol(spr_img, "test");
		ee::ImageSprite* spr_sprite = new ee::ImageSprite(spr_symbol);

		float angle = 0;
		if (pic.src.h != pic.dst.h) {
			assert(pic.src.h == pic.dst.w);
			angle = -SM_PI * 0.5f;
		}
		spr_sprite->SetAngle(angle);

		sm::vec2 pos;
		pos.x = pic.src.x + pic.src.w * 0.5f;
		pos.y = pic.src.y + pic.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		spr_sprite->SetPosition(pos);
		
		ee::InsertSpriteSJ::Instance()->Insert(spr_sprite);

		img_data->RemoveReference();
		spr_sprite->RemoveReference();
		spr_symbol->RemoveReference();
		spr_img->RemoveReference();
	}

	img->RemoveReference();
}

//void RectCutLoader::LoadToDtex(const std::string& pack_file, const std::string& img_name)
//{
//	std::vector<Picture> pictures;
////	LoadJsonFile(pack_file, img_name, pictures);
//	LoadRRPFile(pack_file, 5837, pictures);
//
//	std::string dir = ee::FileHelper::getFileDir(pack_file);
//	ee::Image* img = ee::ImageMgr::Instance()->GetItem(dir + "\\pack.png");
//
////	ee::ImageClip clip(*img_data);
//	ee::DynamicTexAndFont* dtex = ee::DynamicTexAndFont::Instance();
//	for (int i = 0, n = pictures.size(); i < n; ++i)
//	{
//		const Picture& pic = pictures[i];
//		sm::rect r_src, r_dst;
//		r_src.combine(sm::vec2(pic.src.x, pic.src.y));
//		r_src.combine(sm::vec2(pic.src.x+pic.src.w, pic.src.y+pic.src.h));
//		r_dst.combine(sm::vec2(pic.dst.x, pic.dst.y));
//		r_dst.combine(sm::vec2(pic.dst.x+pic.dst.w, pic.dst.y+pic.dst.h));
//		dtex->AddImageWithRegion(img, r_src, r_dst, pic.src.h != pic.dst.h);
//	}
//	dtex->EndImageWithRegion();
//
//	img->RemoveReference();
//}

void RectCutLoader::LoadJsonFile(const std::string& pack_file, const std::string& img_name,
								 std::vector<Picture>& pictures)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(pack_file.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value val = value["parts"][i++];
	while (!val.isNull()) {
		std::string path = val["filepath"].asString();
		if (path.find(img_name) != std::string::npos) {
			Picture s;

			s.src.x = val["src"]["x"].asInt();
			s.src.y = val["src"]["y"].asInt();
			s.src.w = val["src"]["w"].asInt();
			s.src.h = val["src"]["h"].asInt();

			s.dst.x = val["dst"]["x"].asInt();
			s.dst.y = val["dst"]["y"].asInt();
			s.dst.w = val["dst"]["w"].asInt();
			s.dst.h = val["dst"]["h"].asInt();

			s.filepath = path;

			pictures.push_back(s);
		}
		val = value["parts"][i++];
	}
}

void RectCutLoader::LoadRRPFile(const std::string& pack_file, int img_id,
								std::vector<Picture>& pictures)
{
	pictures.clear();

	std::locale::global(std::locale(""));
	std::ifstream fin(pack_file.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	int pic_sz = 0;
	fin.read(reinterpret_cast<char*>(&pic_sz), sizeof(int32_t));
	pictures.reserve(pic_sz);
	for (int i = 0; i < pic_sz; ++i)
	{
		int id = 0;
		fin.read(reinterpret_cast<char*>(&id), sizeof(int16_t));
		int sz = 0;
		fin.read(reinterpret_cast<char*>(&sz), sizeof(int16_t));
		fin.seekg(2*2, fin.cur); // skip w and h
		if (id == img_id) {
			for (int i = 0; i < sz; ++i)
			{
				Picture pic;

				int16_t si = 0;
				fin.read(reinterpret_cast<char*>(&si), sizeof(int16_t));
				pic.src.x = si;
				fin.read(reinterpret_cast<char*>(&si), sizeof(int16_t));
				pic.src.y = si;
				fin.read(reinterpret_cast<char*>(&si), sizeof(int16_t));
				pic.dst.x = si;
				fin.read(reinterpret_cast<char*>(&si), sizeof(int16_t));
				pic.dst.y = si;

				int16_t w, h;
				fin.read(reinterpret_cast<char*>(&w), sizeof(int16_t));
				fin.read(reinterpret_cast<char*>(&h), sizeof(int16_t));
				if (w < 0 && h < 0) {
					pic.src.w = -w;
					pic.src.h = -h;
					pic.dst.w = pic.src.h;
					pic.dst.h = pic.src.w;
				} else {
					pic.src.w = w;
					pic.src.h = h;
					pic.dst.w = pic.src.w;
					pic.dst.h = pic.src.h;
				}
				pictures.push_back(pic);
			}
		} else {
			fin.seekg(sizeof(int16_t) * 6 * sz, fin.cur);
		}
	}

	fin.close();
}

}