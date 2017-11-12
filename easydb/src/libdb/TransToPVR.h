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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransToPVR(); }

private:
	void Trigger(const std::string& path);

	void EncodeByDtexPvr(const std::string& filepath) const;
	void EncodeByPvrTexTool(const std::string& filepath) const;

}; // TransToPVR

}

#endif // _EASYDB_TRANS_TO_PVR_H_