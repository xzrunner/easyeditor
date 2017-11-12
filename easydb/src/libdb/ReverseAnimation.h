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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ReverseAnimation(); }

private:
	void Trigger(const std::string& dir) const;

}; // ReverseAnimation

}

#endif // _EASYDB_REVERSE_ANIMATION_H_