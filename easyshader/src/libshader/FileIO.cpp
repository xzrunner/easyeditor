#include "FileIO.h"
#include "Shader2D.h"
#include "Shader3D.h"
#include "ToolBarPanel.h"
#include "SliderCtrl.h"

#include <ee/FileHelper.h>
#include <ee/ShaderMgr.h>
#include <ee/Exception.h>

#include <easy3d.h>

#include <json/json.h>

#include <fstream>

namespace eshader
{

static const std::string STR_INT	= "int";
static const std::string STR_BOOL	= "bool";
static const std::string STR_FLOAT	= "float";
static const std::string STR_IVEC2	= "ivec2";
static const std::string STR_IVEC3	= "ivec3";
static const std::string STR_IVEC4	= "ivec4";
static const std::string STR_VEC2	= "vec2";
static const std::string STR_VEC3	= "vec3";
static const std::string STR_MAT2	= "mat2";
static const std::string STR_MAT3	= "mat3";
static const std::string STR_MAT4	= "mat4";
static const std::string STR_TIME	= "time";
static const std::string STR_INPUT	= "input";

Shader* FileIO::LoadShader(const std::string& filepath, ee::StageCanvas* canvas,
						   ToolbarPanel* toolbar, bool is_2d)
{
	toolbar->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	Shader* shader = LoadShader(dir, value, toolbar, is_2d);
	if (is_2d) {
		ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
		shader_mgr->null();
		shader_mgr->SetSpriteShader(static_cast<ee::SpriteShader*>(shader->GetShaderImpl()));
		shader_mgr->sprite();
	} else {
		e3d::ShaderMgr* shader_mgr = e3d::ShaderMgr::Instance();
		shader_mgr->Null();
		shader_mgr->SetModelShader(static_cast<e3d::ModelShader*>(shader->GetShaderImpl()));
		shader_mgr->Model();
	}
	shader->LoadUniforms();

	return shader;
}

void FileIO::StoreShader(const std::string& filepath, const ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	StoreShader(toolbar, value);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Shader* FileIO::LoadShader(const std::string& dir, const Json::Value& value,
						   ToolbarPanel* toolbar, bool is_2d)
{
	std::string vert_path = dir + "\\" + value["vert_path"].asString(),
		frag_path = dir + "\\" + value["frag_path"].asString();
	Shader* shader;
	if (is_2d) {
		shader = new Shader2D(vert_path, frag_path);
	} else {
		shader = new Shader3D(vert_path, frag_path);
	}
	shader->LoadShader();

	// uniforms
	int i = 0;
	Json::Value uniform_val = value["uniforms"][i++];
	while (!uniform_val.isNull()) {
		Uniform* uniform = LoadUniform(uniform_val, toolbar, shader);
		uniform->SetLocation(shader->GetShaderImpl()->GetProgram());
		if (is_2d) {
			Shader2D* shader2d = static_cast<Shader2D*>(shader);
			if (uniform->GetType() == UT_TIME) {
				shader2d->AddTimeUniform(uniform);
			} else if (uniform->GetType() == UT_INPUT) {
				shader2d->AddInputUniform(uniform);
			} else {
				shader->AddUniform(uniform);
			}
		} else {
			shader->AddUniform(uniform);
		}

		uniform_val = value["uniforms"][i++];
	}

	return shader;
}

Uniform* FileIO::LoadUniform(const Json::Value& value, ToolbarPanel* toolbar, Shader* shader)
{
	std::string name = value["name"].asString();
	UniformType type = TransStrToUType(value["type"].asString());
	Uniform* uniform = new Uniform(name, type);
	std::string title = value["title"].asString();
	
	switch (type)
	{
	case UT_INT: case UT_BOOL:
		{
			std::vector<SliderItemInt> items;
			LoadValue(value, 1, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);
		}
		break;
	case UT_FLOAT:
		{
			std::vector<SliderItemFloat> items;
			LoadValue(value, 1, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);
		}
		break;
	case UT_IVEC2:
		{
			std::vector<SliderItemInt> items;
			LoadValue(value, 2, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);
		}
		break;
	case UT_IVEC3:
		{
			std::vector<SliderItemInt> items;
			LoadValue(value, 3, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);
		}
		break;
	case UT_IVEC4:
		{
			std::vector<SliderItemInt> items;
			LoadValue(value, 4, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);
		}
		break;
	case UT_VEC2:
		{
			std::vector<SliderItemFloat> items;
			LoadValue(value, 2, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);		
		}
		break;
	case UT_VEC3:
		{
			std::vector<SliderItemFloat> items;
			LoadValue(value, 3, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);		
		}
		break;
	case UT_VEC4:
		{
			std::vector<SliderItemFloat> items;
			LoadValue(value, 4, items, uniform);
			toolbar->AddUniform(title, name, shader, uniform, items);		
		}
		break;
	}
	return uniform;
}

void FileIO::StoreShader(const ToolbarPanel* toolbar, Json::Value& value)
{
	const std::vector<SliderCtrl*>& sliders = toolbar->GetSliders();
	for (int i = 0, n = sliders.size(); i < n; ++i) {
		double val[16];
		sliders[i]->GetValue(val);
		StoreUniform(val, value["uniforms"][i]);
	}
}

void FileIO::StoreUniform(const double val[16], Json::Value& value)
{
	UniformType type = TransStrToUType(value["type"].asString());
	switch (type)
	{
	case UT_INT: case UT_BOOL:
		StoreValue(val, value["value"], false, 1);
		break;
	case UT_FLOAT:
		StoreValue(val, value["value"], true, 1);
		break;
	case UT_IVEC2:
		StoreValue(val, value["value"], false, 2);
		break;
	case UT_IVEC3:
		StoreValue(val, value["value"], false, 3);
		break;
	case UT_IVEC4:
		StoreValue(val, value["value"], false, 4);
		break;
	case UT_VEC2:
		StoreValue(val, value["value"], true, 2);
		break;
	case UT_VEC3:
		StoreValue(val, value["value"], true, 3);
		break;
	case UT_VEC4:
		StoreValue(val, value["value"], true, 4);
		break;
	}
}

UniformType FileIO::TransStrToUType(const std::string& str)
{
	UniformType type;
	if (str == STR_INT) {
		type = UT_INT;
	} else if (str == STR_BOOL) {
		type = UT_BOOL;
	} else if (str == STR_FLOAT) {
		type = UT_FLOAT;
	} else if (str == STR_IVEC2) {
		type = UT_IVEC2;
	} else if (str == STR_IVEC3) {
		type = UT_IVEC3;
	} else if (str == STR_IVEC4) {
		type = UT_IVEC4;
	} else if (str == STR_VEC2) {
		type = UT_VEC2;
	} else if (str == STR_VEC3) {
		type = UT_VEC3;
	} else if (str == STR_MAT2) {
		type = UT_MAT2;
	} else if (str == STR_MAT3) {
		type = UT_MAT3;
	} else if (str == STR_MAT4) {
		type = UT_MAT4;
	} else if (str == STR_TIME) {
		type = UT_TIME;
	} else if (str == STR_INPUT) {
		type = UT_INPUT;
	} else {
		throw ee::Exception("uniform known type [%s]\n", str);
	}

	return type;
}

std::string FileIO::TransUTypeToStr(UniformType type)
{
	if (type == UT_INT) {
		return STR_INT;
	} else if (type == UT_BOOL) {
		return STR_BOOL;
	} else if (type == UT_FLOAT) {
		return STR_FLOAT;
	} else if (type == UT_IVEC2) {
		return STR_IVEC2;
	} else if (type == UT_IVEC3) {
		return STR_IVEC3;
	} else if (type == UT_IVEC4) {
		return STR_IVEC4;
	} else if (type == UT_VEC2) {
		return STR_VEC2;
	} else if (type == UT_VEC3) {
		return STR_VEC3;
	} else if (type == UT_VEC4) {
		return STR_IVEC4;
	} else if (type == UT_MAT2) {
		return STR_MAT2;
	} else if (type == UT_MAT3) {
		return STR_MAT3;
	} else if (type == UT_MAT4) {
		return STR_MAT4;
	} else if (type == UT_TIME) {
		return STR_TIME;
	} else if (type == UT_INPUT) {
		return STR_INPUT;
	} else {
		throw ee::Exception("uniform known type [%d]\n", type);
	}
}

template <typename T>
void FileIO::LoadValue(const Json::Value& value, int count,
					   std::vector<SliderItem<T> >& items,
					   Uniform* uniform)
{
	if (value["value"].size() != count) {
		std::string msg = ee::StringHelper::Format("uniform [%s] value not fit type [%s]\n", 
			uniform->GetName(), TransUTypeToStr(uniform->GetType()));
		throw ee::Exception(msg);
	}

	// no need sliders
	if (value["region"]["begin"].size() != count || 
		value["region"]["end"].size() != count) {
		int i0 = 0, i1 = 1, i2 = 2, i3 = 3;
		if (count == 1) {
			uniform->Set((T)(value["value"][i0].asDouble()));
		} else if (count == 2) {
			uniform->Set((T)(value["value"][i0].asDouble()),
				(T)(value["value"][i1].asDouble()));
		} else if (count == 3) {
			uniform->Set((T)(value["value"][i0].asDouble()),
				(T)(value["value"][i1].asDouble()),
				(T)(value["value"][i2].asDouble()));
		} else {
			assert(count == 4);
			uniform->Set((T)(value["value"][i0].asDouble()),
				(T)(value["value"][i1].asDouble()),
				(T)(value["value"][i2].asDouble()),
				(T)(value["value"][i3].asDouble()));
		}
		return;
	}

	for (int i = 0; i < count; ++i) {
		std::string name;
		if (count > 1) {
			if (i == 0) name = "x";
			else if (i == 1) name = "y";
			else if (i == 2) name = "z";
			else if (i == 3) name = "w";
		}

		T val = (T)(value["value"][i].asDouble());
		T min = (T)(value["region"]["begin"][i].asDouble()),
		  max = (T)(value["region"]["end"][i].asDouble());
		items.push_back(SliderItem<T>(name, val, min, max));
	}

	if (count == 1) {
		uniform->Set(items[0].default);
	} else if (count == 2) {
		uniform->Set(items[0].default, items[1].default);
	} else if (count == 3) {
		uniform->Set(items[0].default, items[1].default, items[2].default);
	} else {
		assert(count == 4);
		uniform->Set(items[0].default, items[1].default, items[2].default, items[3].default);
	}
}

void FileIO::StoreValue(const double val[16], Json::Value& value, bool is_float, int count)
{
	if (is_float) {
		for (int i = 0; i < count; ++i) {
			value[i] = (float)val[i];
		}
	} else {
		for (int i = 0; i < count; ++i) {
			value[i] = (int)val[i];
		}
	}
}

}