#ifndef _EASYDB_OUTLINE_IMAGE_H_
#define _EASYDB_OUTLINE_IMAGE_H_

#include "ICommand.h"

namespace edb
{

class OutlineImage : public ICommand
{
public:
	OutlineImage() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new OutlineImage(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineImage

}

#endif // _EASYDB_OUTLINE_IMAGE_H_