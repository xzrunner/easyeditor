#ifndef _EASYDB_SCALE_IMAGE_H_
#define _EASYDB_SCALE_IMAGE_H_

#include "ICommand.h"

namespace edb
{

class ScaleImage : public ICommand
{
public:
	ScaleImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new ScaleImage(); }

private:
	void Trigger(const std::string& dir, float scale);

}; // ScaleImage

}

#endif // _EASYDB_SCALE_IMAGE_H_