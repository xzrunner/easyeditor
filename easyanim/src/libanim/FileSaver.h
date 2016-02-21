#ifndef _EASYANIM_FILE_SAVER_H_
#define _EASYANIM_FILE_SAVER_H_

#include "Symbol.h"

#include <json/json.h>

namespace eanim
{

class FileSaver
{
public:
	static void Store(const std::string& filepath, const Symbol& symbol);

private:
	static void Store(Json::Value& value, Symbol::Layer* layer, const std::string& dir);
	static void Store(Json::Value& value, Symbol::Frame* frame, const std::string& dir);
	static void Store(Json::Value& value, ee::Sprite* sprite, const std::string& dir);

}; // FileSaver

}

#endif // _EASYANIM_FILE_SAVER_H_