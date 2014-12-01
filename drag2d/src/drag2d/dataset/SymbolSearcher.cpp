#include "SymbolSearcher.h"
#include "SearcherPathMgr.h"
#include "ISymbol.h"

#include "common/FileNameTools.h"

namespace d2d
{

wxString SymbolSearcher::GetSymbolPath(const wxString& dir, 
									   const Json::Value& sprite_val)
{
	wxString filepath = sprite_val["filepath"].asString();
	wxString real_path = FilenameTools::getAbsolutePath(dir, filepath);
	real_path = real_path.Lower();

	// find from searcher
	SearcherPathMgr* searcher = SearcherPathMgr::Instance();
	if (!searcher->IsExist(real_path)) {
		const Json::Value& filepaths_val = sprite_val["filepaths"];
		if (!filepaths_val.isNull())
		{
			int j = 0;
			Json::Value filepath_val = filepaths_val[j++];
			while (!filepath_val.isNull()) {
				wxString filepath = FilenameTools::getAbsolutePath(dir, filepath_val.asString());
				filepath = filepath.Lower();
				if (searcher->IsExist(filepath)) {
					real_path = filepath;
					break;
				}
				filepath_val = filepaths_val[j++];
			}
		}
	}
	// find from exist files
	if (!FilenameTools::isExist(real_path)) {
		const Json::Value& filepaths_val = sprite_val["filepaths"];
		if (!filepaths_val.isNull()) {
			int i = 0;
			Json::Value filepath_val = filepaths_val[i++];
			while (!filepath_val.isNull()) {
				wxString filepath = FilenameTools::getAbsolutePath(dir, filepath_val.asString());
				if (FilenameTools::isExist(filepath)) {
					real_path = filepath;
					break;
				}
				filepath_val = filepaths_val[i++];
			}
		}
	}

	return real_path;
}

void SymbolSearcher::SetSymbolFilepaths(const wxString& dir, ISymbol* symbol, 
										const Json::Value& sprite_val)
{
	// todo same!

	Json::Value filepaths_val = sprite_val["filepaths"];
	if (!filepaths_val.isNull()) {
		std::vector<std::string> filepaths;
		int i = 0;
		Json::Value filepath_val = filepaths_val[i++];
		while (!filepath_val.isNull()) {
			std::string p = filepath_val.asString();
			filepaths.push_back(p);
			filepath_val = filepaths_val[i++];
		}
		filepaths.push_back(sprite_val["filepath"].asString());
		symbol->SetFilepaths(filepaths);
	}
}

}