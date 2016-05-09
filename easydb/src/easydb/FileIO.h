#ifndef _EASYDB_FILE_IO_H_
#define _EASYDB_FILE_IO_H_

#include <json/json.h>

namespace ee { class Sprite; class Shape; }

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

	static Json::Value Store(ee::Sprite* sprite, const sm::vec2& offset);
	static Json::Value Store(ee::Shape* shape, const sm::vec2& offset);

}; // FileIO

}

#endif // _EASYDB_FILE_IO_H_