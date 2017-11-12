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
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new AddFrameSprite(); }

private:
	void Trigger(const std::string& dir, const std::string& sprite_path) const;

}; // AddFrameSprite

}

#endif // _EASYDB_ADD_FRAME_SPRITE_H_
