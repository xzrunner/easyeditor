#ifndef EDB_FILE_IO_H
#define EDB_FILE_IO_H

#include <drag2d.h>
#include <json/json.h>

namespace edb
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static d2d::ISprite* loadSprite(const Json::Value& value);
		static d2d::IShape* loadShape(const Json::Value& value);

		static Json::Value store(d2d::ISprite* sprite, const d2d::Vector& offset);
		static Json::Value store(d2d::IShape* shape, const d2d::Vector& offset);

	}; // FileIO
}

#endif // EDB_FILE_IO_H