#include "FileLoader.h"
#include "Symbol.h"

#include "LoadFromJson.h"
#include "LoadFromLua.h"
#include "LoadFromEP.h"
#include "LoadFromPSD.h"
#include "LoadFromBin.h"

#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace ecomplex
{

void FileLoader::Load(const std::string& filepath, Symbol& complex)
{
	auto ext = gum::FilepathHelper::Extension(filepath);
	if (ext == ".bin") {
		LoadFromBin::Load(filepath, complex);
		return;
	}

	if (ee::Config::Instance()->IsUseDTex()) {
//		gum::DTex::Instance()->LoadBegin();
	}

	complex.Clear();

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
		LoadFromEP::Load(value, dir, complex);
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

void FileLoader::LoadChildren(const std::string& filepath, std::vector<std::string>& children)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int type = ee::SymbolFile::Instance()->Type(filepath);
	if (type == s2::SYM_COMPLEX) {
		std::string dir = ee::FileHelper::GetFileDir(filepath);
		LoadFromJson::LoadChildren(value, dir, children);
	}
}

}