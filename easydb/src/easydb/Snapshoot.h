#ifndef _EASYDB_SNAPSHOOT_H_
#define _EASYDB_SNAPSHOOT_H_

#include "ICommand.h"

namespace edb
{

class Snapshoot : public ICommand
{
public:
	Snapshoot() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new Snapshoot(); }

private:
	void Run(const std::string& srcdir, const std::string& dstdir) const;

}; // Snapshoot

}

#endif // _EASYDB_SNAPSHOOT_H_