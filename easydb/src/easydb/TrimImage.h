#ifndef _EASYDB_TRIM_IMAGE_H_
#define _EASYDB_TRIM_IMAGE_H_

#include "ICommand.h"

namespace edb
{

class TrimImage : public ICommand
{
public:
	TrimImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TrimImage(); }

private:
	void Trigger(const std::string& dir);

}; // TrimImage

}

#endif // _EASYDB_TRIM_IMAGE_H_