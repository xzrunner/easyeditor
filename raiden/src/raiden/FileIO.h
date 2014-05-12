#ifndef RAIDEN_FILE_IO_H
#define RAIDEN_FILE_IO_H

#include <json/json.h>

#include <drag2d.h>

namespace raiden
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static d2d::Layer* loadLayer(const Json::Value& value,
			const std::string& dir);
		static Json::Value store(const d2d::Layer* layer,
			const std::string& dir);

		static d2d::ISprite* loadSprite(const Json::Value& value,
			const std::string& dir);
		static Json::Value store(const d2d::ISprite* sprite,
			const std::string& dir);

	}; // FileIO
}

#endif // RAIDEN_FILE_IO_H