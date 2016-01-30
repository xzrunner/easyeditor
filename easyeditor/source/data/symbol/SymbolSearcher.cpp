#include "SymbolSearcher.h"
#include "SearcherPathMgr.h"
#include "Symbol.h"
#include "FileHelper.h"
#include "StringHelper.h"

namespace ee
{

std::string SymbolSearcher::GetSymbolPath(const std::string& dir, 
										  const Json::Value& sprite_val)
{
	std::string filepath = sprite_val["filepath"].asString();
	std::string real_path = FileHelper::GetAbsolutePath(dir, filepath);
	StringHelper::ToLower(real_path);

	// find from searcher
	SearcherPathMgr* searcher = SearcherPathMgr::Instance();
	if (!searcher->IsExist(real_path)) {
		const Json::Value& filepaths_val = sprite_val["filepaths"];
		if (!filepaths_val.isNull())
		{
			int j = 0;
			Json::Value filepath_val = filepaths_val[j++];
			while (!filepath_val.isNull()) {
				std::string filepath = FileHelper::GetAbsolutePath(dir, filepath_val.asString());
				StringHelper::ToLower(filepath);
				if (searcher->IsExist(filepath)) {
					real_path = filepath;
					break;
				}
				filepath_val = filepaths_val[j++];
			}
		}
	}
	// find from exist files
	if (!FileHelper::IsFileExist(real_path)) {
		const Json::Value& filepaths_val = sprite_val["filepaths"];
		if (!filepaths_val.isNull()) {
			int i = 0;
			Json::Value filepath_val = filepaths_val[i++];
			while (!filepath_val.isNull()) {
				std::string filepath = FileHelper::GetAbsolutePath(dir, filepath_val.asString());
				if (FileHelper::IsFileExist(filepath)) {
					real_path = filepath;
					break;
				}
				filepath_val = filepaths_val[i++];
			}
		}
	}

	return real_path;
}

void SymbolSearcher::SetSymbolFilepaths(const std::string& dir, Symbol* symbol, 
										const Json::Value& sprite_val)
{
	// todo same!

	Json::Value filepaths_val = sprite_val["filepaths"];
	if (!filepaths_val.isNull()) {
		std::set<std::string> filepaths;
		int i = 0;
		Json::Value filepath_val = filepaths_val[i++];
		while (!filepath_val.isNull()) {
			std::string p = filepath_val.asString();
			filepaths.insert(p);
			filepath_val = filepaths_val[i++];
		}
		filepaths.insert(sprite_val["filepath"].asString());

		symbol->SetFilepaths(filepaths);
	}
}

}