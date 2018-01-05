#include "ModelFile.h"

#include <node3/ModelType.h>

#include <algorithm>

namespace e3d
{

CU_SINGLETON_DEFINITION(ModelFile);

ModelFile::ModelFile()
{
	Regist(n3::MODEL_OBJECT,    "object3d");
	Regist(n3::MODEL_COMPLEX,   "complex3d");
	Regist(n3::MODEL_ANIMATION, "animation3d");
}

int ModelFile::Type(const std::string& filepath) const
{
	if (filepath.empty()) {
		return n3::MODEL_INVALID;
	}

	int pos = filepath.rfind('.');
	if (pos == -1) {
		return n3::MODEL_INVALID;
	}

	std::string ext = filepath.substr(pos + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "ppm" || ext == "pvr" || ext == "pkm")
	{
		return n3::MODEL_UNKNOWN;
	}
	else if (ext == "json")
	{
		const std::string filename = filepath.substr(0, filepath.find_last_of('.'));
		int pos = filename.find_last_of('_');
		if (pos == -1) {
			return n3::MODEL_UNKNOWN;
		}

		std::string tag = filename.substr(pos + 1);
		std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
		auto itr = m_tag2type.find(tag);
		if (itr != m_tag2type.end()) {
			return itr->second;
		} else {
			return n3::MODEL_UNKNOWN;
		}
	}
	else
	{
		return n3::MODEL_OBJECT;
	}
}

std::string ModelFile::Tag(int type) const
{
	auto itr = m_type2tag.find(type);
	if (itr != m_type2tag.end()) {
		return itr->second;
	} else {
		return "";
	}
}

void ModelFile::Regist(int type, const std::string& tag)
{
	m_type2tag.insert(std::make_pair(type, tag));
	m_tag2type.insert(std::make_pair(tag, type));
}

}