#ifndef _EASYANIM_FILE_SAVER_H_
#define _EASYANIM_FILE_SAVER_H_

#include <sprite2/AnimSymbol.h>

#include <json/json.h>

namespace eanim
{

class Symbol;

class FileSaver
{
public:
	static void Store(const std::string& filepath, const Symbol& symbol);

private:
	static void Store(Json::Value& value, s2::AnimSymbol::Layer* layer, const std::string& dir);
	static void Store(Json::Value& value, s2::AnimSymbol::Frame* frame, const std::string& dir);
	static void Store(Json::Value& value, s2::Sprite* sprite, const std::string& dir);

}; // FileSaver

}

#endif // _EASYANIM_FILE_SAVER_H_