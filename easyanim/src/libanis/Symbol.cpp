#include "Symbol.h"

#include "../libanim/Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Exception.h>

#include <fstream>
#include <algorithm>

namespace libanis
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{

}

sm::rect Symbol::GetBounding(const s2::Sprite* spr/* = NULL*/) const
{
	return m_rect;
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_syms.size(); i < n; ++i) {
		m_syms[i]->ReloadTexture();
	}
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_name = name = value["name"].asString();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);

	int idx = 0;
	Json::Value temp_val = value["template"][idx++];
	while (!temp_val.isNull()) {
		std::string dirpath = temp_val["path"].asString();

		eanim::Symbol* sym = new eanim::Symbol();
		sym->SetFilepath(m_filepath);
		SpriteLoader loader(dirpath);
		sym->LoadFromFile(loader);

		temp_val = value["template"][idx++];
	}

	for (int i = 0, n = m_syms.size(); i < n; ++i) {
		m_rect.Combine(m_syms[i]->GetBounding());
	}
}

void Symbol::Clear()
{
	for_each(m_syms.begin(), m_syms.end(), cu::RemoveRefFunctor<eanim::Symbol>());
	m_syms.clear();
}

//////////////////////////////////////////////////////////////////////////
// class Symbol::SpriteLoader
//////////////////////////////////////////////////////////////////////////

std::string Symbol::SpriteLoader::
GetSymbolPath(const std::string& dir, const Json::Value& val) const
{
	std::string filepath = val["filepath"].asString();
	while (true)
	{
		if (ee::FileHelper::IsFileExist(filepath))
			break;

		std::string absolute_path = ee::FileHelper::GetAbsolutePath(dir, filepath);
		if (ee::FileHelper::IsFileExist(absolute_path))
		{
			filepath = absolute_path;
			break;
		}

		std::string res_path = m_dirpath + "\\" + filepath;
		if (!ee::FileHelper::IsFileExist(res_path)) {
			absolute_path = ee::FileHelper::GetAbsolutePath(dir, res_path);
		}
		if (!ee::FileHelper::IsFileExist(absolute_path)) {
			throw ee::Exception("File: %s don't exist!", res_path.c_str());
		}

		filepath = absolute_path;
		break;
	}
	return filepath;
}

}