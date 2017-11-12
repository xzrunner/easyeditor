#ifndef _EASYDB_PACK_REGULAR_RECT_H_
#define _EASYDB_PACK_REGULAR_RECT_H_

#include "ICommand.h"

namespace edb
{

class PackRegularRect : public ICommand
{
public:
	PackRegularRect() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackRegularRect(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir, const std::string& name);

}; // PackRegularRect

}

#endif // _EASYDB_PACK_REGULAR_RECT_H_