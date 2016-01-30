#include "CharacterFileName.h"

namespace lr
{

CharacterFileName::CharacterFileName(const std::string& filepath)
	: m_filepath(filepath)
{
	m_dir = d2d::FileHelper::GetFileDir(m_filepath);
	std::string filename = d2d::FileHelper::GetFilename(m_filepath);

	int pos = 0, last_pos = 0;
	for (int i = 0; i < 4; ++i) 
	{
		pos = filename.find_first_of("_", last_pos);
		m_fields[i] = filename.substr(last_pos, pos - last_pos);
		last_pos = pos + 1;
	}
	m_fields[FT_POSTFIX] = filename.substr(pos+1);
}

std::string CharacterFileName::ConnectFieldExcept(FieldType field) const
{
	std::string ret;
	for (int i = 0; i < FIELD_COUNT; ++i) {
		if ((FieldType)i != field) {
			ret += m_fields[i];
			if (i != FIELD_COUNT - 1) {
				ret += "_";
			}
		}
	}
	return ret;
}

std::string CharacterFileName::GetOutputName() const
{
	return m_fields[FT_CHARACTER] + "_" + m_fields[FT_ACTION] + "_" + m_fields[FT_COLOR];
}

bool CharacterFileName::FieldSameExcept(const CharacterFileName& name, FieldType field) const
{
	for (int i = 0; i < FIELD_COUNT; ++i) {
		if ((FieldType)i == field) {
			if (m_fields[i] == name.m_fields[i]) {
				return false;
			}
		} else {
			if (m_fields[i] != name.m_fields[i]) {
				return false;
			}
		}
	}
	return true;
}

std::string CharacterFileName::GetFilepathSwitchField(FieldType key, const std::string& val) const
{
	std::string ret = m_dir + "\\";
	for (int i = 0; i < FIELD_COUNT; ++i) {
		if ((int)key != i) {
			ret += m_fields[i];
		} else {
			ret += val;
		}
		if (i != FIELD_COUNT - 1) {
			ret += "_";
		}
	}
	ret += ".json";
	return ret;
}

bool CharacterFileName::IsValidFilepath(const std::string& filepath)
{
	if (!d2d::FileType::IsType(filepath, d2d::FileType::e_complex)) {
		return false;
	}

	std::string filename = d2d::FileHelper::GetFilename(filepath);
	int count = 0;
	for (int i = 0, n = filename.size(); i < n; ++i) {
		if (filename[i] == '_') {
			++count;
		}
	}
	return count == 4;
}

}