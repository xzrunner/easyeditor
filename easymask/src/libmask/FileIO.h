#ifndef _EASYMASK_FILE_IO_H_
#define _EASYMASK_FILE_IO_H_

#include <ee/Symbol.h>

#include <json/json.h>

namespace emask
{

class Symbol;
class FileIO
{
public:
	static void Store(const char* filepath, const Symbol* sym);
	static void Load(const char* filepath, Symbol* sym);

private:
	static Json::Value Store(const std::string& dir, const ee::Sprite* spr);
	static ee::Sprite* Load(const std::string& dir, const Json::Value& val);

}; // FileIO

}

#endif // _EASYMASK_FILE_IO_H_