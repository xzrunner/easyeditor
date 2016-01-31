#include "Symbol.h"

#include "../libanim/Symbol.h"

namespace libanis
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
	Clear();
}

void Symbol::ReloadTexture() const
{
	for (int i = 0, n = m_symbols.size(); i < n; ++i) {
		m_symbols[i]->ReloadTexture();
	}
}

void Symbol::Draw(const ee::Matrix& mt, const ee::ColorTrans& color, 
				  const ee::Sprite* spr, const ee::Sprite* root) const
{
	
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite/* = NULL*/) const
{
	return m_rect;
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

		libanim::Symbol* symbol = new libanim::Symbol();
		symbol->SetFilepath(m_filepath);
		Loader loader(dirpath);
		symbol->LoadFromFile(loader);

		temp_val = value["template"][idx++];
	}

	for (int i = 0, n = m_symbols.size(); i < n; ++i) {
		m_rect.Combine(m_symbols[i]->GetSize());
	}
}

void Symbol::Clear()
{
	for_each(m_symbols.begin(), m_symbols.end(), ee::ReleaseObjectFunctor<libanim::Symbol>());
	m_symbols.clear();
}

//////////////////////////////////////////////////////////////////////////
// class Symbol::Loader
//////////////////////////////////////////////////////////////////////////

std::string Symbol::Loader::
GetSymbolPath(const std::string& dir, const Json::Value& json_val) const
{
	std::string filepath = json_val["filepath"].asString();
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