#include "FileIO.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"
#include "LoadFromBin.h"

#include <fstream>

#include <easyscale9.h>

#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SymbolSearcher.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/SymbolPath.h>

#include <boost/filesystem.hpp>

namespace escale9
{

static const int OFFSET_VERSION = 2;
static const int VERSION = OFFSET_VERSION;

void FileIO::Load(const char* filename, ee::LibraryPanel* library, 
				  ee::MultiSpritesImpl* stage, ToolbarPanel* toolbar)
{
	auto ext = boost::filesystem::extension(filename);
	if (ext == ".bin") {
		LoadFromBin::Load(filename);
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false; 

	std::string dir = ee::FileHelper::GetFileDir(filename);

	bool need_offset = true;
	if (!value["version"].isNull() && value["version"].asInt() >= OFFSET_VERSION) {
		need_offset = false;
	}

 	int i = 0;
 	Json::Value spriteValue = value["sprite"][i++];
 	while (!spriteValue.isNull()) {
		auto& spr = Load(spriteValue, dir);
		if (need_offset) {
			spr->Translate(sm::vec2(-150, -150));
		}
		ee::InsertSpriteSJ::Instance()->Insert(spr);
 		spriteValue = value["sprite"][i++];
 	}

	data.open_image_edge_clip = ori_clip_cfg;

	float width = value["width"].asInt(),
		  height = value["height"].asInt();
	toolbar->setSize(width, height);

	library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::Store(const char* filename, StagePanel* stage, 
				   ToolbarPanel* toolbar)
{
	Json::Value value;

	value["version"] = VERSION;

	value["type"] = stage->getPatchSymbol()->GetScale9().GetType();

	value["width"] = toolbar->getWidth();
	value["height"] = toolbar->getHeight();

 	std::vector<ee::SprPtr> sprs;
	stage->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

	std::string dir = ee::FileHelper::GetFileDir(filename);
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		value["sprite"][i] = StoreNew(sprs[i], dir);
	}

// 	Symbol* sym = stage->getPatchSymbol();
// 	const Scale9Data& s_data = sym->GetScale9Data();
// 	int idx = 0;
// 	for (int i = 0; i < 3; ++i) {
// 		for (int j = 0; j < 3; ++j) {
// 			auto& spr = s_data.GetSprite(i, j);
// 			if (spr) {
// 				value["sprite new"][idx++] = StoreNew(spr, dir);
// 			} else {
// //				value["sprite new"][idx++] = NULL;
// 				idx++;
// 			}
// 		}
// 	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

ee::SprPtr FileIO::Load(const Json::Value& value, const std::string& dir)
{
	std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, value);
	if (!ee::FileHelper::IsFileExist(filepath)) {
		std::string filepath = value["filepath"].asString();
		throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
			dir.c_str(), filepath.c_str());
	}

	auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::SymbolSearcher::SetSymbolFilepaths(dir, *sym, value);
	auto spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(value);

	return spr;
}

Json::Value FileIO::Store(const ee::SprPtr& spr, const std::string& dir)
{
	Json::Value value;
	const auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());

	// filepath
	value["filepath"] = ee::SymbolPath::GetRelativePath(*sym, dir);
	// filepaths
	const std::set<std::string>& filepaths = sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	spr->Store(value);

	return value;
}

Json::Value FileIO::StoreNew(const ee::SprPtr& spr, const std::string& dir)
{
	Json::Value value;
	const auto sym = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol());

	// filepath
	value["filepath"] = ee::SymbolPath::GetRelativePath(*sym, dir);
	// filepaths
	const std::set<std::string>& filepaths = sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	spr->Store(value);

	return value;
}

} // escale9