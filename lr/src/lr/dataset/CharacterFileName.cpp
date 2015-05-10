#include "CharacterFileName.h"

namespace lr
{

CharacterFileName::CharacterFileName(const std::string& filepath)
	: m_filepath(filepath)
{
	std::string filename = d2d::FilenameTools::getFilename(m_filepath);

	int pos = 0, last_pos = 0;
	for (int i = 0; i < 4; ++i) 
	{
		pos = filename.find_first_of("_", last_pos);
		m_fields[i] = filename.substr(last_pos, pos - last_pos);
		last_pos = pos + 1;
	}
	m_fields[FT_POSTFIX] = filename.substr(pos+1);
}

bool CharacterFileName::FieldSame(const CharacterFileName& name, FieldType field) const
{
	return m_fields[field] == name.m_filepath[field];
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

bool CharacterFileName::IsValidFilepath(const std::string& filepath)
{
	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
		return false;
	}

	std::string filename = d2d::FilenameTools::getFilename(filepath);
	int count = 0;
	for (int i = 0, n = filename.size(); i < n; ++i) {
		if (filename[i] == '_') {
			++count;
		}
	}
	return count == 4;
}

}