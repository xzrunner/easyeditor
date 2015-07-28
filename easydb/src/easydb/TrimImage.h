#ifndef _EASYDB_TRIM_IMAGE_H_
#define _EASYDB_TRIM_IMAGE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class TrimImage : public ICommand
{
public:
	TrimImage() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TrimImage(); }

private:
	void Trigger(const std::string& dir);

	void StoreBoundInfo(const d2d::ImageData& img, const d2d::Rect& r, Json::Value& val) const;
	bool IsTransparent(const d2d::ImageData& img, int x, int y) const;

}; // TrimImage

}

#endif // _EASYDB_TRIM_IMAGE_H_