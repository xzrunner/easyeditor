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
	enum COLOR_CHANNEL
	{
		CC_R = 1,
		CC_G = 2,
		CC_B = 4,
	};

	struct Trans
	{
		int type;
		d2d::Colori col_r, col_g, col_b;
		std::string ext;
	};

private:
	void Run(const std::string& cfg_filepath);

private:
	static std::string ParserTrans(const std::string& str, Trans& t);
	static void TransSpr(const Trans& t, d2d::Sprite* spr);

}; // AnimToComplexWithColor

}

#endif // _EASYDB_ANIM_TO_COMPLEX_WITH_COLOR_H_