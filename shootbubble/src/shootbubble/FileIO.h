#ifndef SHOOTBUBBLE_FILE_IO_H_
#define SHOOTBUBBLE_FILE_IO_H_

#include <json/json.h>



namespace shootbubble
{
	class StagePanel;

	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static ee::ISprite* load(const Json::Value& value);
		static Json::Value store(const ee::ISprite* sprite);

	}; // FileIO
}

#endif // SHOOTBUBBLE_FILE_IO_H_