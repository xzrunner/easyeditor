#include "FileIO.h"
#include "Shader.h"
#include "SliderItem.h"
#include "ToolBarPanel.h"

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

void FileIO::LoadShader(const wxString& filepath, d2d::EditPanel* stage,
						ToolbarPanel* toolbar)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.mb_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	shader_mgr->null();
	wxString dir = d2d::FilenameTools::getFileDir(filepath);
	Shader* shader = LoadShader(dir, value, toolbar);
	shader_mgr->SetSpriteShader(shader->GetShaderImpl());
	shader_mgr->sprite();
	shader->LoadUniforms();
	stage->getCanvas()->resetViewport();
}

Shader* FileIO::LoadShader(const wxString& dir, const Json::Value& value,
						   ToolbarPanel* toolbar)
{
	std::string vert_path = dir + "\\" + value["vert_path"].asString(),
		frag_path = dir + "\\" + value["frag_path"].asString();
	Shader* shader = new Shader(vert_path, frag_path);
	shader->Load();

	// uniforms
	int i = 0;
	Json::Value uniform_val = value["uniforms"][i++];
	while (!uniform_val.isNull()) {
		Uniform* uniform = LoadUniform(uniform_val, toolbar);
		uniform->SetLocation(shader->GetShaderImpl()->GetProgram());
		shader->AddUniform(uniform);
		uniform_val = value["uniforms"][i++];
	}

	return shader;
}

Uniform* FileIO::LoadUniform(const Json::Value& value, ToolbarPanel* toolbar)
{
	std::string name = value["name"].asString();
	UniformType type = TransStrToUType(value["type"].asString());
	Uniform* uniform = new Uniform(name, type);

	std::vector<SliderItemFloat> items;

	switch (type)
	{
	case UT_INT:
		break;
	case UT_BOOL:
		break;
	case UT_FLOAT:
		{
			float default = (float)(value["value"].asDouble());
			float min = (float)(value["region"]["begin"].asDouble()),
				  max = (float)(value["region"]["end"].asDouble());
			uniform->Set(default);
			items.push_back(SliderItemFloat("", default, min, max));
		}
		break;
	}

	std::string title = value["title"].asString();
	toolbar->AddUniform(title, name, uniform, items);

	return uniform;
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
	} else {
		throw d2d::Exception("uniform type: %s, error!\n", str);
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
	} else {
		throw d2d::Exception("uniform type: %d, error!\n", type);
	}
}

}