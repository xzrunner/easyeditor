#ifndef _EASYCOMPLEX_LOAD_FROM_EP_H_
#define _EASYCOMPLEX_LOAD_FROM_EP_H_

#include <ee/Image.h>

#include <json/json.h>

#include <stdint.h>

namespace ecomplex
{

class Symbol;

class LoadFromEP
{
public:
	static void Load(const Json::Value& value, const std::string& dir, Symbol& sym);

private:
	static void LoadImages(const std::string& filename, std::vector<ee::ImagePtr>& images);

	static void LoadImage(uint8_t** ptr, std::vector<ee::ImagePtr>& images);

}; // LoadFromEP

}

#endif // _EASYCOMPLEX_LOAD_FROM_EP_H_