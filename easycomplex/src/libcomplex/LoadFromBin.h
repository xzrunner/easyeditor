#ifndef _LIBCOMPLEX_LOAD_FROM_BIN_H_
#define _LIBCOMPLEX_LOAD_FROM_BIN_H_

#include <drag2d.h>

namespace ecomplex
{

class Symbol;

class LoadFromBin
{
public:
	static void Load(const Json::Value& value, const std::string& dir, 
		Symbol* symbol);

private:
	static void LoadImages(const std::string& filename, std::vector<d2d::Image*>& images);

	static void LoadImage(uint8_t** ptr, std::vector<d2d::Image*>& images);

}; // LoadFromBin

}

#endif // _LIBCOMPLEX_LOAD_FROM_BIN_H_