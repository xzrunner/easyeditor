#ifndef _EASYDB_CALCULATE_IMAGE_AREA_H_
#define _EASYDB_CALCULATE_IMAGE_AREA_H_

#include "ICommand.h"



namespace edb
{

class CalImageArea : public ICommand
{
public:
	CalImageArea() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new CalImageArea(); }

private:
	void Trigger(const std::string& dir);

}; // CalImageArea

}

#endif // _EASYDB_CALCULATE_IMAGE_AREA_H_