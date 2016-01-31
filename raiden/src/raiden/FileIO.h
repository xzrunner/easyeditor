#ifndef RAIDEN_FILE_IO_H
#define RAIDEN_FILE_IO_H

#include <json/json.h>



namespace raiden
{
	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static ee::Layer* loadLayer(const Json::Value& value,
			const std::string& dir);
		static Json::Value store(const ee::Layer* layer,
			const std::string& dir);

		static ee::ISprite* loadSprite(const Json::Value& value,
			const std::string& dir);
		static Json::Value store(const ee::ISprite* sprite,
			const std::string& dir);

	}; // FileIO
}

#endif // RAIDEN_FILE_IO_H