#ifndef _EASYDB_TRANS_TO_GIF_H_
#define _EASYDB_TRANS_TO_GIF_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class TransToGif : public ICommand
{
public:
	TransToGif() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransToGif(); }

private:
	void Run(d2d::Snapshoot& ss, const std::string& srcdir, const std::string& dstdir) const;

}; // TransToGif

}

#endif // _EASYDB_TRANS_TO_GIF_H_