#include "TextureMgr.h"

namespace libcoco
{
namespace epd
{

TextureMgr::TextureMgr()
{
	memset(m_textures, 0, sizeof(int)*MAX_NUM);
}

TextureMgr::~TextureMgr()
{
	for (size_t i = 0; i < MAX_NUM; ++i) {
		delete m_textures[i];
	}
}

bool TextureMgr::Add(const std::string& filepath, int index)
{
	if (index >= MAX_NUM) return false;

	for (size_t i = 0; i < MAX_NUM; ++i) {
		if (m_textures[i] && m_textures[i]->filepath == filepath) {
			return false;
		}
	}

	Entry* entry = new Entry(m_src_data_dir);
	entry->filepath = filepath;
	entry->adapter.Load(filepath.c_str());
	delete m_textures[index];
	m_textures[index] = entry;
	return true;
}

const TextureMgr::Entry* TextureMgr::GetTexture(int idx) const
{
	if (idx < 0 || idx >= MAX_NUM) {
		return NULL;
	} else {
		return m_textures[idx];
	}
}

}
}