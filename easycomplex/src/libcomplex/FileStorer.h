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
	static void Store(const char* filepath, const Symbol* symbol);
	static void StoreWithHistory(const char* filepath, const Symbol* symbol);

private:
	static void CenterSymbol(Symbol* symbol);
	static Json::Value Store(ee::Sprite* sprite, const std::string& dir);

}; // FileStorer

}

#endif // _EASYCOMPLEX_FILE_STORER_H_