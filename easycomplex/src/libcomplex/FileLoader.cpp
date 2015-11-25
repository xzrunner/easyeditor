#include "FileLoader.h"
#include "Symbol.h"

#include "LoadFromJson.h"
#include "LoadFromLua.h"
#include "LoadFromBin.h"

namespace ecomplex
{

//void Symbol::loadResources()
//{
//	bool use_dtex = d2d::Config::Instance()->IsUseDTex();
//	d2d::DynamicTexAndFont* dtex = NULL;
//	if (use_dtex) {
//		dtex = d2d::DynamicTexAndFont::Instance();
//		dtex->BeginImage();
//	}
//
//	clear();
//
//	Json::Value value;
//	Json::Reader reader;
//	std::locale::global(std::locale(""));
//	std::ifstream fin(m_filepath.fn_str());
//	std::locale::global(std::locale("C"));
//	reader.parse(fin, value);
//	fin.close();
//
//	name = value["name"].asString();
//
//	m_clipbox.xMin = value["xmin"].asInt();
//	m_clipbox.xMax = value["xmax"].asInt();
//	m_clipbox.yMin = value["ymin"].asInt();
//	m_clipbox.yMax = value["ymax"].asInt();
//
//	m_use_render_cache = value["use_render_cache"].asBool();
//
// 	wxString dir = d2d::FilenameTools::getFileDir(m_filepath);
//	int i = 0;
//	Json::Value spriteValue = value["sprite"][i++];
//	while (!spriteValue.isNull()) {
//		wxString path = d2d::FilenameTools::getAbsolutePath(dir, spriteValue["filepath"].asString());
//		ISymbol* symbol = NULL;
//		std::string real_filepath = path;
//		try {
//			symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
//		} catch (d2d::Exception& e) {
// 			symbol = NULL;
// 			Json::Value filepaths_val = spriteValue["filepaths"];
// 			if (!filepaths_val.isNull()) 
// 			{
// 				int j = 0;
// 				Json::Value filepath_val = filepaths_val[j++];
// 				while (!filepath_val.isNull() && !symbol) {
//					real_filepath = filepath_val.asString();
// 					wxString filepath = d2d::FilenameTools::getAbsolutePath(dir, real_filepath);
//					filepath_val = filepaths_val[j++];
// 					try {
// 						symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
// 					} catch (d2d::Exception& e) {
// 						symbol = NULL;
// 					}
// 				}
// 			}
//
//			if (!symbol) {
//				throw e;
//			}
//		}
//
// 		// load symbol filepaths
// 		Json::Value filepaths_val = spriteValue["filepaths"];
// 		if (!filepaths_val.isNull()) {
// 			std::vector<std::string> filepaths;
// 			int i = 0;
// 			Json::Value filepath_val = filepaths_val[i++];
// 			while (!filepath_val.isNull()) {
//				std::string p = filepath_val.asString();
//				if (p != real_filepath) {
//					filepaths.push_back(p);
//				}
// 				filepath_val = filepaths_val[i++];
// 			}
//			filepaths.push_back(spriteValue["filepath"].asString());
// 			symbol->SetFilepaths(filepaths);
// 		}
//
////		symbol->refresh();
//		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
//		sprite->load(spriteValue);
//
//		symbol->Release();
//
//		m_sprites.push_back(sprite);
//		spriteValue = value["sprite"][i++];
//	}	
//
//	initBounding();
//
//	if (use_dtex) {
//		dtex->EndImage();
//		if (m_use_render_cache) {
//			dtex->InsertSymbol(*this);
//		}
//	}
//}

void FileLoader::Load(const std::string& filepath, Symbol* complex)
{
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->LoadBegin();
	}

	complex->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	if (!value["lua desc"].isNull()) {
		LoadFromLua::Load(value, dir, complex);
	} else if (!value["bin file"].isNull()) {
		LoadFromBin::Load(value, dir, complex);
	} else {
		LoadFromJson::Load(value, dir, complex);
	}

	complex->InitBounding();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->LoadEnd();

// 		if (complex->m_use_render_cache) {
// 			dtex->InsertSymbol(*complex);
// 		}
	}
}

}