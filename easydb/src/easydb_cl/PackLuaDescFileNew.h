#ifndef _EASYDB_PACK_LUA_DESC_FILE_NEW_H_
#define _EASYDB_PACK_LUA_DESC_FILE_NEW_H_

#include "ICommand.h"

#include <drag2d.h>
#include <easycoco.h>

namespace edb
{

class PackLuaDescFileNew : public ICommand
{
public:
	PackLuaDescFileNew() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackLuaDescFileNew(); }

	void Trigger(const std::string& json_dir, const std::string& tp_json,
		const std::string& tp_dir, const std::string& out_file);

}; // PackLuaDescFileNew

}

#endif // _EASYDB_PACK_LUA_DESC_FILE_NEW_H_