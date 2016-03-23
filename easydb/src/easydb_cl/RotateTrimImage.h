#ifndef _EASYDB_ROTATE_TRIM_IMAGE_H_
#define _EASYDB_ROTATE_TRIM_IMAGE_H_

#include "ICommand.h"

namespace ee { class Snapshoot; class Image; class Vector; }

namespace edb
{

class RotateTrimImage : public ICommand
{
public:
	RotateTrimImage() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new RotateTrimImage(); }

	static const char* GetOutputFileName();

private:
	void RotateTrim(ee::Snapshoot& ss, const std::string& dir);	

	bool GetRotateTrimInfo(const ee::Image* image, 
		int& width, int& height,
		ee::Vector& center, float& angle) const;

}; // RotateTrimImage

}

#endif // _EASYDB_ROTATE_TRIM_IMAGE_H_