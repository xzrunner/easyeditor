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

void Symbol::Draw(const d2d::Matrix& mt,
				const d2d::Colorf& mul, 
				const d2d::Colorf& add,
				const d2d::Colorf& r_trans,
				const d2d::Colorf& g_trans,
				const d2d::Colorf& b_trans,
				const d2d::ISprite* sprite/* = NULL*/) const
{
	
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return m_rect;
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_name = name = value["name"].asString();

	std::string dir = d2d::FilenameTools::getFileDir(m_filepath);

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
		m_rect.combine(m_symbols[i]->GetSize());
	}
}

void Symbol::Clear()
{
	for_each(m_symbols.begin(), m_symbols.end(), d2d::ReleaseObjectFunctor<libanim::Symbol>());
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
		if (d2d::FilenameTools::isExist(filepath))
			break;

		std::string absolute_path = d2d::FilenameTools::getAbsolutePath(dir, filepath);
		if (d2d::FilenameTools::isExist(absolute_path))
		{
			filepath = absolute_path;
			break;
		}

		std::string res_path = m_dirpath + "/" + filepath;
		if (!d2d::FilenameTools::isExist(res_path)) {
			absolute_path = d2d::FilenameTools::getAbsolutePath(dir, res_path);
		}
		if (!d2d::FilenameTools::isExist(absolute_path)) {
			throw d2d::Exception("File: %s don't exist!", res_path.c_str());
		}

		filepath = absolute_path;
		break;
	}
	return filepath;
}

}