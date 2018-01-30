#ifndef _S2LOADER_TEXT_TABLE_LOADER_H_
#define _S2LOADER_TEXT_TABLE_LOADER_H_

#include <cu/cu_stl.h>
#include <sprite2/config.h>
#ifndef S2_TEXT_TABLE_DB
#include <bimp/FileLoader.h>
#endif // S2_TEXT_TABLE_DB

#include <string>

namespace s2loader
{

class TextTableLoader
{
public:
#ifndef S2_TEXT_TABLE_DB
	static void LoadFromCSV(const CU_STR& filepath);

	static void LoadFromBin(const CU_STR& filepath);
#else
	static void LoadFromDB(const CU_STR& index_path, 
		const CU_STR& data_path);
#endif // S2_TEXT_TABLE_DB

private:
#ifndef S2_TEXT_TABLE_DB
	class LanguagesLoader : public bimp::FileLoader
	{
	public:
		LanguagesLoader(const CU_STR& filepath);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	}; // LanguagesLoader
#endif // S2_TEXT_TABLE_DB

}; // TextTableLoader

}

#endif // _S2LOADER_TEXT_TABLE_LOADER_H_
