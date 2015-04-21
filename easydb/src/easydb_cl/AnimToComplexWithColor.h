#ifndef _EASYDB_ANIM_TO_COMPLEX_WITH_COLOR_H_
#define _EASYDB_ANIM_TO_COMPLEX_WITH_COLOR_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class AnimToComplexWithColor : public ICommand
{
public:
	AnimToComplexWithColor() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new AnimToComplexWithColor(); }

private:
	void Run(const std::string& cfg_filepath);

}; // AnimToComplexWithColor

}

#endif // _EASYDB_ANIM_TO_COMPLEX_WITH_COLOR_H_