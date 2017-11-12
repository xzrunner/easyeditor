#ifndef _EASYDB_SCALE_IMAGE_H_
#define _EASYDB_SCALE_IMAGE_H_

#include "ICommand.h"

namespace edb
{

class ScaleImage : public ICommand
{
public:
	ScaleImage() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ScaleImage(); }

private:
	void Scale(const std::string& dir, float scale);
	void Scale(const std::string& src, const std::string& dst, float scale);

}; // ScaleImage

}

#endif // _EASYDB_SCALE_IMAGE_H_