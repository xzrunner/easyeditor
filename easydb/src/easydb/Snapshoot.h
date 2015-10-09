#ifndef _EASYDB_SNAPSHOOT_H_
#define _EASYDB_SNAPSHOOT_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class Snapshoot : public ICommand
{
public:
	Snapshoot() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new Snapshoot(); }

private:
	void Run(d2d::Snapshoot& ss, const std::string& srcdir, const std::string& dstdir) const;

}; // Snapshoot

}

#endif // _EASYDB_SNAPSHOOT_H_