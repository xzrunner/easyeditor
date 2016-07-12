#include "FileIO.h"
#include "Symbol.h"
#include "Sprite.h"
#include "ChangeWindowViewSizeSJ.h"

#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FetchAllVisitor.h>

#include <easycomplex.h>

#include <fstream>

namespace eui
{
namespace window
{

void FileIO::Store(const char* filepath, const Symbol* sym)
{
	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	value["name"] = sym->name;

	sym->GetAnchorMgr().StoreToFile(value, dir);

	value["view"]["width"] = sym->GetWidth();
	value["view"]["height"] = sym->GetHeight();

// 	std::vector<ee::Sprite*> sprites;
// 	const_cast<Symbol*>(sym)->GetAnchorMgr().Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
// 	value["wrapper filepath"] = StoreWrapper(filepath, sym->name, sprites);

	StoreRefs(value, sym, dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::Load(const char* filepath, Symbol* sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	sym->name = value["name"].asString();

	if (!value["view"].isNull()) {
		int w = value["view"]["width"].asInt(),
			h = value["view"]["height"].asInt();
		sym->SetWidth(w);
		sym->SetHeight(h);
		sym->GetAnchorMgr().OnViewChanged(w, h);
		ChangeWindowViewSizeSJ::Instance()->Change(w, h);
	}

	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";
	LoadSprites(value, sym, dir);
	LoadRefs(value, sym, dir);
}

std::string FileIO::StoreWrapper(const std::string& filepath, const std::string& name,
								 const std::vector<ee::Sprite*>& sprites)
{
	std::string dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	ecomplex::Symbol wrapper_complex;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		wrapper_complex.Add(sprites[i]);
	}
	std::string filename = filepath.substr(0, filepath.find_last_of('_'));
	std::string wrapper_path = filename + "_wrapper_complex[gen].json";
	wrapper_complex.SetFilepath(wrapper_path);
	wrapper_complex.name = name;
	ecomplex::FileStorer::Store(wrapper_path.c_str(), &wrapper_complex);

	return ee::FileHelper::GetRelativePath(dir, wrapper_path);
}

void FileIO::LoadSprites(const Json::Value& val, Symbol* sym, const std::string& dir)
{
	AnchorMgr& anchors = sym->GetAnchorMgr();

	int idx = 0;
	Json::Value spr_val = val["sprite"][idx++];
	while (!spr_val.isNull()) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);
		anchors.Insert(sprite);
		sprite->Release();
		symbol->Release();

		spr_val = val["sprite"][idx++];
	}
}

void FileIO::StoreRefs(Json::Value& val, const Symbol* sym, const std::string& dir)
{
	const std::vector<Sprite*>& sprites = sym->GetExtRefs();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Sprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = ee::FileHelper::GetRelativePath(dir,
			spr->GetSymbol().GetFilepath());
		spr->Store(spr_val);
		val["ref_spr"][i] = spr_val;

		FileIO::Store(spr->GetSymbol().GetFilepath().c_str(), &spr->GetSymbol());
	}
}

void FileIO::LoadRefs(const Json::Value& val, Symbol* sym, const std::string& dir)
{
	int idx = 0;
	Json::Value spr_val = val["ref_spr"][idx++];
	while (!spr_val.isNull()) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);
		sym->InsertExtRef(static_cast<Sprite*>(sprite));
		sprite->Release();
		symbol->Release();

		spr_val = val["ref_spr"][idx++];
	}
}

}
}