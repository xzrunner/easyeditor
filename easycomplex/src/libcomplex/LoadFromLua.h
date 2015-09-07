#ifndef _LIBCOMPLEX_LOAD_FROM_LUA_H_
#define _LIBCOMPLEX_LOAD_FROM_LUA_H_

// #include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class Symbol;

class LoadFromLua
{
public:
	static void Load(const Json::Value& value, const std::string& dir, 
		Symbol* complex);

private:
	static void LoadImages(const std::string& name, std::vector<d2d::Image*>& images);

}; // LoadFromLua

}

#endif // _LIBCOMPLEX_LOAD_FROM_LUA_H_