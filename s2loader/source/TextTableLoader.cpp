#include "s2loader/TextTableLoader.h"

#include <sprite2/TextTable.h>
#include <bimp/ImportStream.h>
#include <gum/StringHelper.h>

#include <fstream>

namespace s2loader
{

#ifndef S2_TEXT_TABLE_DB
void TextTableLoader::LoadFromCSV(const CU_STR& filepath)
{
	auto text_table = s2::TextTable::Instance();

	text_table->Clear();

	std::ifstream fin(filepath.c_str());
	CU_STR str_line;
	getline(fin, str_line);	// skip header
	while (getline(fin, str_line))
	{
		CU_VEC<CU_STR> items;
		gum::StringHelper::Split(str_line, ",", items);

		CU_VEC<CU_STR> langs;
		copy(items.begin() + 1, items.end(), back_inserter(langs));
		for (auto& lang : langs) {
			gum::StringHelper::Replace(lang, "\\n", "\n");
		}

		text_table->Insert(items[0], langs);
	}
	fin.close();
}

void TextTableLoader::LoadFromBin(const CU_STR& filepath)
{
	LanguagesLoader loader(filepath);
	loader.Load();
}

#else

void TextTableLoader::LoadFromDB(const CU_STR& index_path,
	                             const CU_STR& data_path)
{
	s2::TextTable::Instance()->LoadDB(index_path, data_path);
}

#endif // S2_TEXT_TABLE_DB

#ifndef S2_TEXT_TABLE_DB

/************************************************************************/
/* class TextTableLoader::LanguagesLoader                               */
/************************************************************************/

TextTableLoader::LanguagesLoader::LanguagesLoader(const CU_STR& filepath)
	: FileLoader(filepath)
{
}

void TextTableLoader::LanguagesLoader::OnLoad(bimp::ImportStream& is)
{
	auto text_table = s2::TextTable::Instance();

	text_table->Clear();

	int items_count = is.UInt16();
	int types_count = is.UInt16();

	CU_VEC<CU_STR> header;
	header.reserve(items_count);
	for (int i = 0; i < types_count; ++i) {
		header.push_back(is.String());
	}
	text_table->SetTypes(header);

	for (int i = 0; i < items_count; ++i) 
	{
		CU_STR tid = is.LongString();

		CU_VEC<CU_STR> langs;
		langs.reserve(types_count);
		for (int j = 0; j < types_count; ++j) {
			langs.push_back(is.LongString());
		}

		text_table->Insert(tid, langs);
	}
}

#endif // S2_TEXT_TABLE_DB

}