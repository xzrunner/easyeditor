#include "RectCutLoader.h"
#include "StagePanel.h"

#include <drag2d.h>
#include <easyimage.h>

#include <JSON/json.h>
#include <fstream>

namespace ecomplex
{

RectCutLoader::RectCutLoader(StagePanel* stage)
	: m_stage(stage)
{
}

void RectCutLoader::LoadOnlyJson(const wxString& pack_file, const wxString& img_name)
{
	std::vector<Sprite> sprites;
	LoadJsonFile(pack_file, img_name, sprites);

	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		const Sprite& s = sprites[i];
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(s.filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);

		d2d::Vector pos;
		pos.x = s.src.x + s.src.w * 0.5f;
		pos.y = s.src.y + s.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		sprite->setTransform(pos, sprite->getAngle());

		m_stage->insertSprite(sprite);

		sprite->Release();
		symbol->Release();
	}
}

void RectCutLoader::LoadJsonAndImg(const wxString& pack_file, const wxString& img_name)
{
	std::vector<Sprite> sprites;
	LoadJsonFile(pack_file, img_name, sprites);

	wxString dir = d2d::FilenameTools::getFileDir(pack_file);
	d2d::Image* img = d2d::ImageMgr::Instance()->getItem(dir + "\\pack.png");

	eimage::ImageClip clip(img);
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{		
		const Sprite& spr = sprites[i];
		const byte* pixels = clip.Clip(spr.dst.x, spr.dst.x + spr.dst.w, 1024 - spr.dst.y - spr.dst.h, 1024 - spr.dst.y);
		d2d::Image* spr_img = new d2d::Image(pixels, spr.dst.w, spr.dst.h);
		d2d::ImageSymbol* spr_symbol = new d2d::ImageSymbol(spr_img, "test");
		d2d::ImageSprite* spr_sprite = new d2d::ImageSprite(spr_symbol);

		float angle = 0;
		if (spr.src.h != spr.dst.h) {
			assert(spr.src.h == spr.dst.w);
			angle = -d2d::PI * 0.5f;
		}

		d2d::Vector pos;
		pos.x = spr.src.x + spr.src.w * 0.5f;
		pos.y = spr.src.y + spr.src.h * 0.5f;
		pos.x += 400;
		pos.y += 400;
		spr_sprite->setTransform(pos, angle);

		m_stage->insertSprite(spr_sprite);

		spr_sprite->Release();
		spr_symbol->Release();
		spr_img->Release();
	}

	img->Release();
}

void RectCutLoader::LoadJsonFile(const wxString& pack_file, const wxString& img_name,
								 std::vector<Sprite>& sprites)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(pack_file.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value val = value["sprites"][i++];
	while (!val.isNull()) {
		wxString path = val["filepath"].asString();
		if (path.Contains(img_name)) {
			Sprite s;

			s.src.x = val["src"]["x"].asInt();
			s.src.y = val["src"]["y"].asInt();
			s.src.w = val["src"]["w"].asInt();
			s.src.h = val["src"]["h"].asInt();

			s.dst.x = val["dst"]["x"].asInt();
			s.dst.y = val["dst"]["y"].asInt();
			s.dst.w = val["dst"]["w"].asInt();
			s.dst.h = val["dst"]["h"].asInt();

			s.filepath = path;

			sprites.push_back(s);
		}
		val = value["sprites"][i++];
	}
}

}