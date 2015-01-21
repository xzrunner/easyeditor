#ifndef _EASYDB_PNG2PVR_H_
#define _EASYDB_PNG2PVR_H_

#include "ICommand.h"

namespace edb
{

class PNG2PVR : public ICommand
{
public:
	PNG2PVR() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PNG2PVR(); }

private:
	void Trigger(const std::string& dir);

}; // PNG2PVR

}

#endif // _EASYDB_PNG2PVR_H_