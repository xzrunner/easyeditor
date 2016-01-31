#ifndef _EASYDB_REVERSE_ANIMATION_H_
#define _EASYDB_REVERSE_ANIMATION_H_

#include "ICommand.h"

namespace edb
{

class ReverseAnimation : public ICommand
{
public:
	ReverseAnimation() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new ReverseAnimation(); }

private:
	void Trigger(const std::string& dir) const;

}; // ReverseAnimation

}

#endif // _EASYDB_REVERSE_ANIMATION_H_