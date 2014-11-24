#ifndef _EASYDB_OUTLINE_TO_POLYGOIN_H_
#define _EASYDB_OUTLINE_TO_POLYGOIN_H_

#include "ICommand.h"

namespace edb
{

class OutlineToPolygon : public ICommand
{
public:
	OutlineToPolygon() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new OutlineToPolygon(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineToPolygon

}

#endif // _EASYDB_OUTLINE_TO_POLYGOIN_H_