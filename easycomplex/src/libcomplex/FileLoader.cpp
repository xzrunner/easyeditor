#include "FileLoader.h"
#include "Symbol.h"

#include "LoadFromJson.h"
#include "LoadFromLua.h"
#include "LoadFromBin.h"
#include "LoadFromPSD.h"

#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <fstream>

namespace ecomplex
{

void FileLoader::Load(const std::string& filepath, Symbol* complex)
{
	if (ee::Config::Instance()->IsUseDTex()) {
//		gum::DTex::Instance()->LoadBegin();
	}

	complex->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int type = ee::SymbolFile::Instance()->Type(filepath);
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	if (!value["lua desc"].isNull()) {
		LoadFromLua::Load(value, dir, complex);
	} else if (!value["bin file"].isNull()) {
		LoadFromBin::Load(value, dir, complex);
	} else if (type == s2::SYM_COMPLEX) {
		LoadFromJson::Load(filepath, value, dir, complex);
	} else if (type == ee::SYM_PSD) {
		LoadFromPSD::Load(value, dir, complex);
	}

//	complex->InitBounding();

	if (ee::Config::Instance()->IsUseDTex()) {
//		gum::DTex::Instance()->LoadEnd();

// 		if (complex->m_use_render_cache) {
// 			dtex->InsertSymbol(*complex);
// 		}
	}
}

}