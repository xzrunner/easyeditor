#ifndef SHOOTBUBBLE_FILE_IO_H_
#define SHOOTBUBBLE_FILE_IO_H_

#include <json/json.h>

#include <drag2d.h>

namespace shootbubble
{
	class StagePanel;

	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static d2d::ISprite* load(const Json::Value& value);
		static Json::Value store(const d2d::ISprite* sprite);

	}; // FileIO
}

#endif // SHOOTBUBBLE_FILE_IO_H_