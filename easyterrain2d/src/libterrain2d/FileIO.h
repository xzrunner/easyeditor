#ifndef _EASYTERRAIN2D_FILE_IO_H_
#define _EASYTERRAIN2D_FILE_IO_H_

#include <JSON/json.h>

namespace eterrain2d
{

class OceanMesh;
class Symbol;

class FileIO
{
public:
	static void StoreOceanMesh(const OceanMesh* ocean, const std::string& dir, Json::Value& value);
	static OceanMesh* LoadOceanMesh(const std::string& dir, const Json::Value& value);

	static void StoreSymbol(const char* filepath, const Symbol* symbol);

}; // FileIO

}

#endif // _EASYTERRAIN2D_FILE_IO_H_