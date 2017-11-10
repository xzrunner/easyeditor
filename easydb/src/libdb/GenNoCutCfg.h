#ifndef _EASYDB_GEN_NO_CUT_CFG_H_
#define _EASYDB_GEN_NO_CUT_CFG_H_

#include "ICommand.h"

#include <json/json.h>

#include <set>

namespace edb
{

class GenNoCutCfg : public ICommand
{
public:
	GenNoCutCfg();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new GenNoCutCfg(); }	

private:
	void Trigger(const std::string& src_dir, const std::string& cfg_path);

	static void LoadConfig(const std::string& src_dir, const Json::Value& src, 
		const std::string& key, std::set<std::string>& dst);

	static void AddPath(std::set<std::string>& dst, const std::string& src,
		const std::string& src_dir);

}; // GenNoCutCfg

}

#endif // _EASYDB_GEN_NO_CUT_CFG_H_