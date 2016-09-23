#ifndef _EASYCOMPLEX_FILE_STORER_H_
#define _EASYCOMPLEX_FILE_STORER_H_

#include <json/json.h>

namespace ee { class Sprite; }

namespace ecomplex
{

class Symbol;

class FileStorer
{
public:
	static void Store(const char* filepath, const Symbol* sym);
	static void StoreWithHistory(const char* filepath, const Symbol* sym);

private:
	static void CenterSymbol(Symbol* sym);
	static Json::Value Store(ee::Sprite* spr, const std::string& dir);

	static void CheckDuplicateName(const Symbol* sym);
	static void CheckNameDiff(const Symbol* sym);

	static void StoreAction(const Symbol* sym, Json::Value& val);

}; // FileStorer

}

#endif // _EASYCOMPLEX_FILE_STORER_H_