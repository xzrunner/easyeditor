#ifndef EDB_FILE_IO_H
#define EDB_FILE_IO_H


#include <json/json.h>

namespace edb
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static ee::Sprite* loadSprite(const Json::Value& value);
		static ee::Shape* loadShape(const Json::Value& value);

		static Json::Value store(ee::Sprite* sprite, const ee::Vector& offset);
		static Json::Value store(ee::Shape* shape, const ee::Vector& offset);

	}; // FileIO
}

#endif // EDB_FILE_IO_H