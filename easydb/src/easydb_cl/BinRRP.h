#ifndef _EASYDB_BIN_RRP_H_
#define _EASYDB_BIN_RRP_H_

#include "ICommand.h"

namespace edb
{

class BinRRP : public ICommand
{
public:
	BinRRP() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinRRP(); }

private:
	void Trigger(const std::string& filepath);

}; // PackRRPBin

}

#endif // _EASYDB_BIN_RRP_H_