#ifndef _EASYDB_REGULAR_RECT_CUT_H_
#define _EASYDB_REGULAR_RECT_CUT_H_

#include "ICommand.h"



namespace edb
{

class RegularRectCut : public ICommand
{
public:
	RegularRectCut() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new RegularRectCut(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir);

}; // RegularRectCut

}

#endif // _EASYDB_REGULAR_RECT_CUT_H_