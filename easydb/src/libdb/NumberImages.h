#ifndef _EASYDB_NUMBER_IAMGES_H_
#define _EASYDB_NUMBER_IAMGES_H_

#include "ICommand.h"

namespace edb
{

class NumberImages : public ICommand
{
public:
	NumberImages() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new NumberImages(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_file);

}; // NumberImages

}

#endif // _EASYDB_NUMBER_IAMGES_H_