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
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new OutlineToTriStrip(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineToTriStrip

}

#endif // _EASYDB_OUTLINE_TO_TRIANGLE_STRPS_H_