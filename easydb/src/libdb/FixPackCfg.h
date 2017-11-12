#ifndef _EASYDB_FIX_PACK_CFG_H_
#define _EASYDB_FIX_PACK_CFG_H_

#include "ICommand.h"

namespace edb
{

class FixPackCfg : public ICommand
{
public:
	FixPackCfg() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixPackCfg(); }

private:
	void Fix(const std::string& filepath);

}; // FixPackCfg

}

#endif // _EASYDB_FIX_PACK_CFG_H_