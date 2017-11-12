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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new OutlineImage(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineImage

}

#endif // _EASYDB_OUTLINE_IMAGE_H_