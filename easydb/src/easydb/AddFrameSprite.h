#ifndef _EASYDB_ADD_FRAME_SPRITE_H_
#define _EASYDB_ADD_FRAME_SPRITE_H_

#include "ICommand.h"

namespace edb
{

class AddFrameSprite : public ICommand
{
public:
	AddFrameSprite() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new AddFrameSprite(); }

private:
	void Trigger(const std::string& dir, const std::string& sprite_path) const;

}; // AddFrameSprite

}

#endif // _EASYDB_ADD_FRAME_SPRITE_H_
