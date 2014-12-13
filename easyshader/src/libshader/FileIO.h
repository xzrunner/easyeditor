#ifndef _DRAG2D_FILE_IO_H_
#define _DRAG2D_FILE_IO_H_

#include <drag2d.h>
#include <JSON/value.h>

#include "Uniform.h"

namespace eshader
{

class Shader;

class FileIO
{
public:
	static void LoadShader(const wxString& filepath, d2d::EditPanel* stage);

private:
	static Shader* LoadShader(const wxString& dir, const Json::Value& value);
	static Uniform* LoadUniform(const Json::Value& value);

	static UniformType TransStrToUType(const std::string& str);
	static std::string TransUTypeToStr(UniformType type);

}; // FileIO

}

#endif // _DRAG2D_FILE_IO_H_