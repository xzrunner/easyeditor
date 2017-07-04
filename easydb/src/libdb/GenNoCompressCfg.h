#ifndef _EASYDB_GEN_NO_COMPRESS_CFG_H_
#define _EASYDB_GEN_NO_COMPRESS_CFG_H_

#include "ICommand.h"

#include <json/json.h>

#include <set>

namespace edb
{

class GenNoCompressCfg : public ICommand
{
public:
	GenNoCompressCfg();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new GenNoCompressCfg(); }	

private:
	void Trigger(const std::string& src_dir, const std::string& src_cfg,
		const std::string& dst_dir);

	static void AddPath(std::set<std::string>& dst, const std::string& dst_dir,
		const Json::Value& src, const std::string& src_dir);

}; // GenNoCompressCfg

}

#endif // _EASYDB_GEN_NO_COMPRESS_CFG_H_