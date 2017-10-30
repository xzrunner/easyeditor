#ifndef _EASYCOMPLEX_FILE_STORER_H_
#define _EASYCOMPLEX_FILE_STORER_H_

#include <ee/Sprite.h>

#include <cu/cu_stl.h>
#include <json/json.h>

namespace ecomplex
{

class Symbol;

class FileStorer
{
public:
	static void Store(const std::string& filepath, const Symbol& sym, const std::string& dir);
	static void StoreWithHistory(const std::string& filepath, const Symbol& sym, const std::string& dir);

private:
	static void CenterSymbol(Symbol& sym);
	static Json::Value Store(const ee::SprPtr& spr, const std::string& dir);

	static void CheckName(const Symbol& sym);

	static void CheckDuplicateName(const Symbol& sym);
	static void CheckNameDiff(const Symbol& sym);
	static void CheckAnchorName(const Symbol& sym);

//	static void StoreAction(const Symbol& sym, Json::Value& val);

}; // FileStorer

}

#endif // _EASYCOMPLEX_FILE_STORER_H_