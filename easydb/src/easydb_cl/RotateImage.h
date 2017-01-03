#ifndef _EASYDB_ROTATE_IMAGE_H_
#define _EASYDB_ROTATE_IMAGE_H_

#include "ICommand.h"

namespace edb
{

class RotateImage : public ICommand
{
public:
	RotateImage() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new RotateImage(); }

private:
	void Rotate(const std::string& src_dir, const std::string& dst_dir);

}; // RotateImage

}

#endif // _EASYDB_ROTATE_IMAGE_H_