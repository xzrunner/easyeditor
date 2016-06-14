#ifndef _EASYCOMPLEX_LOAD_FROM_PSD_H_
#define _EASYCOMPLEX_LOAD_FROM_PSD_H_

#include <json/json.h>

namespace ee { class Sprite; }

namespace ecomplex
{

class Symbol;

class LoadFromPSD
{
public:
	static void Load(const Json::Value& value, const std::string& dir, 
		Symbol* complex);

private:
	static ee::Sprite* LoadItem(const Json::Value& value, const std::string& dir, int tw, int th);	

	static ee::Sprite* LoadGroup(const Json::Value& value, const std::string& dir, int tw, int th);
	static ee::Sprite* LoadLayer(const Json::Value& value, const std::string& dir, int tw, int th);

}; // LoadFromPSD

}

#endif // _EASYCOMPLEX_LOAD_FROM_PSD_H_