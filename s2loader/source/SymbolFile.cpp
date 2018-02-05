#include "s2loader/SymbolFile.h"

#include <sprite2/SymType.h>
#include <gum/StringHelper.h>

#include <assert.h>

namespace s2loader
{

CU_SINGLETON_DEFINITION(SymbolFile);

SymbolFile::SymbolFile()
{
	Regist(s2::SYM_SCALE9,		"scale9");
	Regist(s2::SYM_ICON,		"icon");
	Regist(s2::SYM_TEXTURE,		"texture");
	Regist(s2::SYM_TEXTBOX,		"text");
	Regist(s2::SYM_COMPLEX,		"complex");
	Regist(s2::SYM_ANIMATION,	"anim");
	Regist(s2::SYM_PARTICLE3D,	"particle");
	Regist(s2::SYM_PARTICLE2D,	"particle2d");
	Regist(s2::SYM_SHAPE,		"shape");
	Regist(s2::SYM_MESH,		"mesh");
	Regist(s2::SYM_MASK,		"mask");
	Regist(s2::SYM_TRAIL,		"trail");
	Regist(s2::SYM_SKELETON,	"skeleton");

	UNKNOWN_TAG = "unknown";
}

int SymbolFile::Type(const CU_STR& filepath) const
{
	if (filepath.empty()) {
		return s2::SYM_INVALID;
	}

	int pos = filepath.rfind('.');
	if (pos == -1) {
		return s2::SYM_INVALID;
	}

	CU_STR ext = filepath.substr(pos + 1);
	// not ext, md5
	if (ext.size() > 10) {
		ext = filepath.substr(0, pos);
		ext = ext.substr(ext.find_last_of('.') + 1);
	}
	gum::StringHelper::ToLower(ext);
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "ppm" || ext == "pvr" || ext == "pkm") 
	{
		return s2::SYM_IMAGE;
	}
	else if (ext == "json" || ext == "bin") 
	{
		const CU_STR filename = filepath.substr(0, filepath.find_last_of('.'));
		int pos = filename.find_last_of('_');
		if (pos == -1) {
			return s2::SYM_UNKNOWN;
		}

		CU_STR tag = filename.substr(pos + 1);
		gum::StringHelper::ToLower(tag);
		CU_MAP<CU_STR, int>::const_iterator itr = m_tag2type.find(tag);
		if (itr != m_tag2type.end()) {
			return itr->second;
		} else {
			return s2::SYM_UNKNOWN;
		}
	}
	else if (ext == "mp3")
	{
		return s2::SYM_AUDIO;
	}
	else if (ext == "x" || ext == "obj" || ext == "param")
	{
		return s2::SYM_MODEL;
	}
	return s2::SYM_UNKNOWN;
}

const CU_STR& SymbolFile::Tag(int type) const
{
	CU_MAP<int, CU_STR>::const_iterator itr = m_type2tag.find(type);
	if (itr != m_type2tag.end()) {
		return itr->second;
	} else {
		return UNKNOWN_TAG;
	}
}

void SymbolFile::Regist(int type, const CU_STR& tag)
{
	m_type2tag.insert(std::make_pair(type, tag));
	m_tag2type.insert(std::make_pair(tag, type));
}

}