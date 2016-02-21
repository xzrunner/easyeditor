#ifndef _EASYDB_FILE_IO_H_
#define _EASYDB_FILE_IO_H_

#include <json/json.h>

namespace ee { class Sprite; class Shape; class Vector; }

namespace edb
{

class FileIO
{
public:
	static void Load(const char* filename);
	static void Store(const char* filename);

private:
	static ee::Sprite* LoadSprite(const Json::Value& value);
	static ee::Shape* LoadShape(const Json::Value& value);

	static Json::Value Store(ee::Sprite* sprite, const ee::Vector& offset);
	static Json::Value Store(ee::Shape* shape, const ee::Vector& offset);

}; // FileIO

}

#endif // _EASYDB_FILE_IO_H_