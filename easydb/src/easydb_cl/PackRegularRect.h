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
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackRegularRect(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir, const std::string& name);

}; // PackRegularRect

}

#endif // _EASYDB_PACK_REGULAR_RECT_H_