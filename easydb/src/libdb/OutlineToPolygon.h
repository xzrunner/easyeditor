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
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new OutlineToPolygon(); }

private:
	void Trigger(const std::string& dir) const;

}; // OutlineToPolygon

}

#endif // _EASYDB_OUTLINE_TO_POLYGOIN_H_