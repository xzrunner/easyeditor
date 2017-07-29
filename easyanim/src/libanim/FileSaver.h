#ifndef _LIBANIM_FILE_SAVER_H_
#define _LIBANIM_FILE_SAVER_H_

#include <sprite2/AnimSymbol.h>
#include <sprite2/AnimLerp.h>

#include <json/json.h>

namespace libanim
{

class Symbol;

class FileSaver
{
public:
	static void Store(const std::string& filepath, const Symbol& sym);

	static Json::Value StoreLerps(const std::vector<std::pair<s2::AnimLerp::SprData, s2::ILerp*> >& lerps);

private:
	static void Store(Json::Value& value, s2::AnimSymbol::Layer* layer, const std::string& dir);
	static void Store(Json::Value& value, s2::AnimSymbol::Frame* frame, const std::string& dir);
	static void Store(Json::Value& value, s2::Sprite* spr, const std::string& dir);

}; // FileSaver

}

#endif // _LIBANIM_FILE_SAVER_H_