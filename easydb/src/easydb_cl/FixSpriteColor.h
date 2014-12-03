#ifndef _EASYDB_FIX_SPRITE_COLOR_H_
#define _EASYDB_FIX_SPRITE_COLOR_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <wx/wx.h>
#include <JSON/json.h>

namespace edb
{

class FixSpriteColor : public ICommand, private FixJsonBase
{
public:
	FixSpriteColor() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixSpriteColor(); }

protected:
	virtual bool FixSprite(const wxString& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixSpriteColor 

}

#endif // _EASYDB_FIX_SPRITE_COLOR_H_
