#ifndef _EASYDB_TRANS_TO_GIF_H_
#define _EASYDB_TRANS_TO_GIF_H_

#include "ICommand.h"

namespace edb
{

class TransToGif : public ICommand
{
public:
	TransToGif() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new TransToGif(); }

private:
	void Run(const std::string& srcdir, const std::string& dstdir) const;

}; // TransToGif

}

#endif // _EASYDB_TRANS_TO_GIF_H_