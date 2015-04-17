#include "FileIO.h"

#include "view/StagePanel.h"

namespace lr
{

void FileIO::load(const char* filename, StagePanel* stage,
				  d2d::LibraryPanel* library)
{
// 	d2d::SymbolMgr::Instance()->clear();
// 	d2d::BitmapMgr::Instance()->clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	wxString dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = NULL;
		try {
			symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		} catch (d2d::Exception& e) {
			std::cout << "Symbol::loadResources error! File:" << filepath << std::endl;
			std::cout << e.what();
		}

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spr_val);
		stage->insertSprite(sprite);

		sprite->Release();
		symbol->Release();

		spr_val = value["sprite"][i++];
	}

//	library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage)
{
	Json::Value value;

	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	wxString dir = d2d::FilenameTools::getFileDir(filename) + "\\";
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(spr_val);

		value["sprite"][i] = spr_val;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

}