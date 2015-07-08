#include "RectCutLoader.h"
#include "StagePanel.h"

#include <drag2d.h>
#include <easyimage.h>

#include <json/json.h>
#include <fstream>

namespace ecomplex
{

RectCutLoader::RectCutLoader(StagePanel* stage)
	: m_stage(stage)
{
}

void RectCutLoader::LoadOnlyJson(const wxString& pack_file, const wxString& img_name)
{
	std::vector<Picture> pictures;
	LoadJsonFile(pack_file, img_name, pictures);

	for (int i = 0, n = pictures.size(); i < n; ++i)
	{
		const Picture& s = pictures[i];
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(s.filepath.ToStdString());
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);

		d2d::Vector pos;
		pos.x = s.src.x + s.src.w * 0.5f;
		pos.y = s.src.y + s.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		sprite->setTransform(pos, sprite->getAngle());

		m_stage->InsertSprite(sprite);

		sprite->Release();
		symbol->Release();
	}
}

void RectCutLoader::LoadJsonAndImg(const wxString& pack_file, const wxString& img_name)
{
	std::vector<Picture> pictures;
	LoadJsonFile(pack_file, img_name, pictures);

	std::string dir = d2d::FilenameTools::getFileDir(pack_file);
	d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(dir + "\\pack.png");

	eimage::ImageClip clip(img);
	for (int i = 0, n = pictures.size(); i < n; ++i)
	{		
		const Picture& pic = pictures[i];
		const uint8_t* pixels = clip.Clip(pic.dst.x, pic.dst.x + pic.dst.w, 1024 - pic.dst.y - pic.dst.h, 1024 - pic.dst.y);
		d2d::Image* spr_img = new d2d::Image(pixels, pic.dst.w, pic.dst.h);
		d2d::ImageSymbol* spr_symbol = new d2d::ImageSymbol(spr_img, "test");
		d2d::ImageSprite* spr_sprite = new d2d::ImageSprite(spr_symbol);

		float angle = 0;
		if (pic.src.h != pic.dst.h) {
			assert(pic.src.h == pic.dst.w);
			angle = -d2d::PI * 0.5f;
		}

		d2d::Vector pos;
		pos.x = pic.src.x + pic.src.w * 0.5f;
		pos.y = pic.src.y + pic.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		spr_sprite->setTransform(pos, angle);

		m_stage->InsertSprite(spr_sprite);

		spr_sprite->Release();
		spr_symbol->Release();
		spr_img->Release();
	}

	img->Release();
}

void RectCutLoader::LoadToDtex(const wxString& pack_file, const wxString& img_name)
{
	std::vector<Picture> pictures;
//	LoadJsonFile(pack_file, img_name, pictures);
	LoadRRPFile(pack_file, 5837, pictures);

	std::string dir = d2d::FilenameTools::getFileDir(pack_file);
	d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(dir + "\\pack.png");

	eimage::ImageClip clip(img);
	d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();
	for (int i = 0, n = pictures.size(); i < n; ++i)
	{
		const Picture& pic = pictures[i];
		d2d::Rect r_src, r_dst;
		r_src.combine(d2d::Vector(pic.src.x, pic.src.y));
		r_src.combine(d2d::Vector(pic.src.x+pic.src.w, pic.src.y+pic.src.h));
		r_dst.combine(d2d::Vector(pic.dst.x, pic.dst.y));
		r_dst.combine(d2d::Vector(pic.dst.x+pic.dst.w, pic.dst.y+pic.dst.h));
		dtex->AddImageWithRegion(img, r_src, r_dst, pic.src.h != pic.dst.h);
	}
	dtex->EndImageWithRegion();

	img->Release();
}

void RectCutLoader::LoadJsonFile(const wxString& pack_file, const wxString& img_name,
								 std::vector<Picture>& pictures)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(pack_file.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value val = value["parts"][i++];
	while (!val.isNull()) {
		wxString path = val["filepath"].asString();
		if (path.Contains(img_name)) {
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

void RectCutLoader::LoadRRPFile(const wxString& pack_file, int img_id,
								std::vector<Picture>& pictures)
{
	pictures.clear();

	std::ifstream fin(pack_file.mb_str(), std::ios::binary);

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