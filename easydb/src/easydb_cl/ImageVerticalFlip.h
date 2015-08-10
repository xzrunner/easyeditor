#ifndef _EASYDB_IMAGE_VERTICAL_FLIP_H_
#define _EASYDB_IMAGE_VERTICAL_FLIP_H_

#include "ICommand.h"

namespace edb
{

class ImageVerticalFlip : public ICommand
{
public:
	ImageVerticalFlip() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new ImageVerticalFlip(); }

private:
	void Trigger(const std::string& dir) const;

}; // ImageVerticalFlip

}

#endif // _EASYDB_IMAGE_VERTICAL_FLIP_H_