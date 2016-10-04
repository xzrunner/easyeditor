#include "SymbolFile.h"
#include "SymbolType.h"

#include <sprite2/SymType.h>
#include <gum/SymbolFile.h>

namespace ee
{

SINGLETON_DEFINITION(SymbolFile);

static const char* TAG_GEN = "gen";

SymbolFile::SymbolFile()
{
	Regist(SYM_ANIS,		"anis");
	Regist(SYM_FONTBLANK,	"fontblank");
	Regist(SYM_FREETYPE,	"freetype");
	Regist(SYM_SCRIPTS,		"scripts");
	Regist(SYM_P3DINV,		"p3dinv");
	Regist(SYM_EJOY2D,		"ejoy2d");
	Regist(SYM_TERRAIN2D,	"terrain2d");
	Regist(SYM_SHADOW,		"shadow");
	Regist(SYM_UI,			"ui");
	Regist(SYM_PSD,			"psd");
	Regist(SYM_UIWND,		"uiwnd");
}

int SymbolFile::Type(const std::string& filepath) const
{
	int type = gum::SymbolFile::Instance()->Type(filepath);
	if (type != s2::SYM_UNKNOWN) {
		return type;
	}
	
	int pos = filepath.rfind('.');
	std::string ext = filepath.substr(pos + 1);
	if (ext == "json")
	{
		const std::string filename = filepath.substr(0, filepath.find_last_of('.'));
		int pos = filename.find_last_of('_');
		if (pos == -1) {
			return s2::SYM_UNKNOWN;
		}

		std::string tag = filename.substr(pos + 1);
		pos = tag.find(TAG_GEN);
		if (pos != std::string::npos) {
			tag = tag.substr(0, pos - 1);
		}

		std::map<std::string, int>::const_iterator itr = m_tag2type.find(tag);
		if (itr != m_tag2type.end()) {
			return itr->second;
		} else {
			return s2::SYM_UNKNOWN;
		}
	}
	else if (ext == "lua")
	{
		const std::string name = filepath.substr(0, filepath.find_last_of('.'));
		if (name.find('_') == -1) {
			return s2::SYM_UNKNOWN;
		}
		const std::string json_ext = name.substr(name.find_last_of('_') + 1);
		if (json_ext == "scripts") {
			return SYM_SCRIPTS;
		} else {
			return s2::SYM_UNKNOWN;
		}
	}
	else if (ext == "ttf")
	{
		return SYM_FREETYPE;
	}

	return s2::SYM_UNKNOWN;
}

const std::string& SymbolFile::Tag(int type) const
{
	const std::string& tag = gum::SymbolFile::Instance()->Tag(type);
	if (tag != gum::SymbolFile::UNKNOWN_TAG) {
		return tag;
	}

	std::map<int, std::string>::const_iterator itr = m_type2tag.find(type);
	if (itr != m_type2tag.end()) {
		return itr->second;
	} else {
		return gum::SymbolFile::UNKNOWN_TAG;
	}
}

void SymbolFile::Regist(int type, const std::string& tag)
{
	m_type2tag.insert(std::make_pair(type, tag));
	m_tag2type.insert(std::make_pair(tag, type));
}

}