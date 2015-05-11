#ifndef _LR_CHARACTER_FILE_NAME_H_
#define _LR_CHARACTER_FILE_NAME_H_

#include <drag2d.h>

namespace lr
{

class CharacterFileName
{
public:
	enum FieldType
	{
		FT_CHARACTER = 0,
		FT_ACTION,
		FT_DIRECTION,
		FT_COLOR,
		FT_POSTFIX
	};

public:
	CharacterFileName(const std::string& filepath);

	const std::string& GetFilepath() const {
		return m_filepath;
	}

	const std::string& GetField(FieldType field) const {
		return m_fields[field];
	}
	std::string ConnectFieldExcept(FieldType field) const;

	bool FieldSame(const CharacterFileName& name, FieldType field) const {
		m_fields[field] == name.m_filepath[field];
	}
	bool FieldSameExcept(const CharacterFileName& name, FieldType field) const;

	std::string GetFilepathSwitchField(FieldType key, const std::string& val) const;

	static bool IsValidFilepath(const std::string& filepath);

private:
	static const int FIELD_COUNT = 5;

private:
	std::string m_dir;
	std::string m_filepath;

	std::string m_fields[FIELD_COUNT];

}; // CharacterFileName

}

#endif // _LR_CHARACTER_FILE_NAME_H_