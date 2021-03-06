#ifndef _EASYDB_PACK_LUA_DESC_FILE_H_
#define _EASYDB_PACK_LUA_DESC_FILE_H_

#include "ICommand.h"


#include <easycoco.h>

namespace edb
{

class PackLuaDescFile : public ICommand
{
public:
	PackLuaDescFile() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackLuaDescFile(); }

	void Trigger(const std::string& json_dir, const std::string& tp_json,
		const std::string& tp_dir, const std::string& out_file);

private:
	void LoadJsonFiles(const std::string& dir);
	void LoadTexPacker(const std::string& tp_json,
		const std::string& tp_dir);

private:
	std::vector<ee::SymPtr> m_syms;

	ecoco::epd::TextureMgr m_tex_mgr;

}; // PackLuaDescFile

}

#endif // _EASYDB_PACK_LUA_DESC_FILE_H_