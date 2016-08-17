#ifndef _EASYTEXPACKER_FILE_IO_H_
#define _EASYTEXPACKER_FILE_IO_H_

#include <json/json.h>

namespace ee { class Sprite; }

namespace etexpacker
{

class StagePanel;

class FileIO
{
public:
	static void Load(const char* filename);
	static void Store(const char* filename);

private:
	static void LoadFromEasypackerFile(const char* filename);
	static void LoadFromTexPackerFile(const char* filename);

	static void StoreToEasypackerFile(const char* filename);
	static void StoreToTexPackerFile(const char* filename);

	static void StoreImage(const char* filename);
	static void StoreEasypackerPosition(const char* filename);
	static void StoreTexpackerPosition(const char* filename);

	static Json::Value Store(const ee::Sprite* spr);

}; // FileIO

}

#endif // _EASYTEXPACKER_FILE_IO_H_