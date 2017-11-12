#ifndef _EASYDB_CALC_IMAGE_AREA_H_
#define _EASYDB_CALC_IMAGE_AREA_H_

#include "ICommand.h"

namespace edb
{

class CalcImageArea : public ICommand
{
public:
	CalcImageArea() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new CalcImageArea(); }

private:
	void Run(const std::string& dir) const;

}; // CalcImageArea

}

#endif // _EASYDB_CALC_IMAGE_AREA_H_