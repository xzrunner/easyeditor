#ifndef _EASYCOMPLEX_LOAD_FROM_BIN_H_
#define _EASYCOMPLEX_LOAD_FROM_BIN_H_

#include <json/json.h>

#include <stdint.h>

namespace ee { class Image; }

namespace ecomplex
{

class Symbol;

class LoadFromBin
{
public:
	static void Load(const Json::Value& value, const std::string& dir, 
		Symbol* sym);

private:
	static void LoadImages(const std::string& filename, std::vector<ee::Image*>& images);

	static void LoadImage(uint8_t** ptr, std::vector<ee::Image*>& images);

}; // LoadFromBin

}

#endif // _EASYCOMPLEX_LOAD_FROM_BIN_H_