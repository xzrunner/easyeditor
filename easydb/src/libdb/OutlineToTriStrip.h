#ifndef _EASYDB_OUTLINE_TO_TRIANGLE_STRPS_H_
#define _EASYDB_OUTLINE_TO_TRIANGLE_STRPS_H_

#include "ICommand.h"

namespace edb
{

class OutlineToTriStrip : public ICommand
{
public:
	OutlineToTriStrip() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new OutlineToTriStrip(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineToTriStrip

}

#endif // _EASYDB_OUTLINE_TO_TRIANGLE_STRPS_H_