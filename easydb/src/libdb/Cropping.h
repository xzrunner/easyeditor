#ifndef _EASYDB_CROPPING_H_
#define _EASYDB_CROPPING_H_

#include "ICommand.h"

namespace edb
{

class Cropping : public ICommand
{
public:
	Cropping() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new Cropping(); }

private:
	void Trigger(const std::string& src_path, const std::string& dst_path,
		int xmin, int ymin, int xmax, int ymax);

}; // Cropping

}

#endif // _EASYDB_CROPPING_H_