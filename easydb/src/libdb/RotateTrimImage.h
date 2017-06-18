#ifndef _EASYDB_ROTATE_TRIM_IMAGE_H_
#define _EASYDB_ROTATE_TRIM_IMAGE_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <stdint.h>

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
	void RotateTrim(const std::string& dir);	

	bool GetRotateTrimInfo(const uint8_t* pixels, int img_w, int img_h, 
		int& width, int& height, sm::vec2& center, float& angle) const;

}; // RotateTrimImage

}

#endif // _EASYDB_ROTATE_TRIM_IMAGE_H_