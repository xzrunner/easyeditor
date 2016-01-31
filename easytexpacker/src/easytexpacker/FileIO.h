#ifndef _EASYTEXPACKER_FILE_IO_H_
#define _EASYTEXPACKER_FILE_IO_H_

#include <json/json.h>



namespace etexpacker
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

		static Json::Value store(const ee::Sprite* sprite);



	}; // FileIO
}

#endif // _EASYTEXPACKER_FILE_IO_H_