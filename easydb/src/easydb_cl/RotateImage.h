#ifndef _EASYDB_ROTATE_IMAGE_H_
#define _EASYDB_ROTATE_IMAGE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class RotateImage : public ICommand
{
public:
	RotateImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new RotateImage(); }

private:
	void Rotate(d2d::Snapshoot& ss, const std::string& src_dir, const std::string& dst_dir);

}; // RotateImage

}

#endif // _EASYDB_ROTATE_IMAGE_H_