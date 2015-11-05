#include "FileIO.h"
#include "TopStagePanel.h"

namespace eui
{

void FileIO::Load(const char* filename, StagePanel* stage, d2d::LibraryPanel* library)
{
//	Json::Value value;
//	Json::Reader reader;
//	std::locale::global(std::locale(""));
//	std::ifstream fin(filename);
//	std::locale::global(std::locale("C"));
//	reader.parse(fin, value);
//	fin.close();
//
//	std::string dir = d2d::FilenameTools::getFileDir(filename) + "\\";
//
//	std::vector<d2d::ISprite*> sprites;
//
//	int idx = 0;
//	Json::Value spr_val = value["sprite"][idx++];
//	while (!spr_val.isNull()) {
//		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
//		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
//
//		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
//		sprite->Load(spr_val);
//
//		stage->InsertSprite(sprite);
//		sprites.push_back(sprite);
//
//		sprite->Release();
//		symbol->Release();
//
//		spr_val = value["sprite"][idx++];
//	}
//
////	stage->GetAnchorMgr()->LoadFromFile(value, sprites);
}

void FileIO::Store(const char* filename, StagePanel* stage)
{
	//std::string dir = d2d::FilenameTools::getFileDir(filename) + "\\";

	//Json::Value value;

	//std::vector<d2d::ISprite*> sprites;
	//stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_ALL);
	//for (int i = 0, n = sprites.size(); i < n; ++i) 
	//{
	//	d2d::ISprite* spr = sprites[i];

	//	Json::Value spr_val;
	//	spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
	//		spr->GetSymbol().GetFilepath()).ToStdString();
	//	spr->Store(spr_val);

	//	value["sprite"][i] = spr_val;
	//}

	//stage->GetAnchorMgr()->StoreToFile(value);

	//Json::StyledStreamWriter writer;
	//std::locale::global(std::locale(""));
	//std::ofstream fout(filename);
	//std::locale::global(std::locale("C"));	
	//writer.write(fout, value);
	//fout.close();
}

}