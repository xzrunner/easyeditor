#ifndef _EASYDB_PACK_LANGUAGE_TABLE_H_
#define _EASYDB_PACK_LANGUAGE_TABLE_H_

#include "ICommand.h"

#include <cu/cu_stl.h>

namespace edb
{

class PackLanguageTable : public ICommand
{
public:
	PackLanguageTable() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackLanguageTable(); }

private:
	void Trigger(const std::string& src_file, const std::string& dst_file);

	void LoadFromCSV(const std::string& filepath);
	void PackToBin(const std::string& filepath);

private:
	CU_VEC<CU_STR> m_header;
	CU_VEC<CU_VEC<CU_STR>> m_body;

}; // PackLanguageTable

}

#endif // _EASYDB_PACK_LANGUAGE_TABLE_H_