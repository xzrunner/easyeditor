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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new RotateImage(); }

private:
	void Rotate(const std::string& src_dir, const std::string& dst_dir);

}; // RotateImage

}

#endif // _EASYDB_ROTATE_IMAGE_H_