#include "RectCutLoader.h"
#include "StagePanel.h"

#include <drag2d.h>

#include <JSON/json.h>
#include <fstream>

namespace ecomplex
{

RectCutLoader::RectCutLoader(StagePanel* stage)
	: m_stage(stage)
{
}

void RectCutLoader::Load(const wxString& pack_file, const wxString& img_name)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(pack_file.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	// load to memory
	std::vector<Sprite> sprites;
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

	// load all sprites
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

}