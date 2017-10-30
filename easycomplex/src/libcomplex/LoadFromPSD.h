#ifndef _EASYCOMPLEX_LOAD_FROM_PSD_H_
#define _EASYCOMPLEX_LOAD_FROM_PSD_H_

#include <ee/Sprite.h>

#include <json/json.h>

namespace ecomplex
{

class Symbol;

class LoadFromPSD
{
public:
	static void Load(const Json::Value& value, const std::string& dir, Symbol& complex);

private:
	static ee::SprPtr LoadItem(const Json::Value& value, const std::string& dir, int tw, int th);	

	static ee::SprPtr LoadGroup(const Json::Value& value, const std::string& dir, int tw, int th);
	static ee::SprPtr LoadLayer(const Json::Value& value, const std::string& dir, int tw, int th);

}; // LoadFromPSD

}

#endif // _EASYCOMPLEX_LOAD_FROM_PSD_H_