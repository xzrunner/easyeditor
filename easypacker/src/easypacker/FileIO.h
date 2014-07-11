#ifndef EPACKER_FILE_IO_H
#define EPACKER_FILE_IO_H

#include <json/json.h>

#include <drag2d.h>

namespace epacker
{
	class StagePanel;

	class FileIO
	{
	public:
		static void load(const char* filename);
		static void store(const char* filename);

	private:
		static void loadFromEasypackerFile(const char* filename);
		static void loadFromTexPackerFile(const char* filename);

		static void storeToEasypackerFile(const char* filename);
		static void storeToTexPackerFile(const char* filename);

		static void storeImage(const char* filename);
		static void storeEasypackerPosition(const char* filename);
		static void storeTexpackerPosition(const char* filename);

		static Json::Value store(const d2d::ISprite* sprite);

	}; // FileIO
}

#endif // EPACKER_FILE_IO_H