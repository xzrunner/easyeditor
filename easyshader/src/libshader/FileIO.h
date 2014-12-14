#ifndef _DRAG2D_FILE_IO_H_
#define _DRAG2D_FILE_IO_H_

#include <drag2d.h>
#include <JSON/value.h>

#include "Uniform.h"
#include "SliderItem.h"

namespace eshader
{

class ToolbarPanel;
class Shader;

class FileIO
{
public:
	static Shader* LoadShader(const wxString& filepath, d2d::GLCanvas* canvas,
		ToolbarPanel* toolbar);
	static void StoreShader(const wxString& filepath, const ToolbarPanel* toolbar);

private:
	static Shader* LoadShader(const wxString& dir, const Json::Value& value,
		ToolbarPanel* toolbar);
	static Uniform* LoadUniform(const Json::Value& value, ToolbarPanel* toolbar,
		Shader* shader);

	static void StoreShader(const ToolbarPanel* toolbar, Json::Value& value);
	static void StoreUniform(const double val[16], Json::Value& value);

	static UniformType TransStrToUType(const std::string& str);
	static std::string TransUTypeToStr(UniformType type);

	template <typename T>
	static void LoadValue(const Json::Value& value, int count,
		std::vector<SliderItem<T> >& int_items, Uniform* uniform);
	static void StoreValue(const double val[16], Json::Value& value,
		bool is_float, int count);

}; // FileIO

}

#endif // _DRAG2D_FILE_IO_H_