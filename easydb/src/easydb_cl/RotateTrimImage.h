#ifndef _EASYDB_ROTATE_TRIM_IMAGE_H_
#define _EASYDB_ROTATE_TRIM_IMAGE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class RotateTrimImage : public ICommand
{
public:
	RotateTrimImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new RotateTrimImage(); }

private:
	void Trigger(const std::string& dir);	

	void GetRotateTrimInfo(const d2d::Image* image, 
		int& width, int& height,
		d2d::Vector& center, float& angle) const;

}; // RotateTrimImage

}

#endif // _EASYDB_ROTATE_TRIM_IMAGE_H_