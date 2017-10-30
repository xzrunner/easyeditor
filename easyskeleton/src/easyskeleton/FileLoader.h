#ifndef _EASYSKELETON_FILE_LOADER_H_
#define _EASYSKELETON_FILE_LOADER_H_

#include <json/json.h>

#include <string>
#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class FileLoader
{
public:
	static void Load(const std::string& filepath, std::vector<ee::SprPtr>& sprs);

private:
	static void LoadSprite(const std::string& dir, const Json::Value& val, std::vector<ee::SprPtr>& sprs);
	
	static void LoadSkeleton(const Json::Value& val, const std::vector<ee::SprPtr>& sprs);

}; // FileLoader

}

#endif // _EASYSKELETON_FILE_LOADER_H_