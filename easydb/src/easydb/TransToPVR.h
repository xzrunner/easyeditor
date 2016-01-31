#ifndef _EASYDB_TRANS_TO_PVR_H_
#define _EASYDB_TRANS_TO_PVR_H_

#include "ICommand.h"

namespace edb
{

class TransToPVR : public ICommand
{
public:
	TransToPVR() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransToPVR(); }

private:
	void Trigger(const std::string& path);

	void EncodeByDtexPvr(const std::string& filepath) const;
	void EncodeByPvrTexTool(const std::string& filepath) const;

}; // TransToPVR

}

#endif // _EASYDB_TRANS_TO_PVR_H_