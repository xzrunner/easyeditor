#ifndef _EASYDB_FIX_SPRITE_COLOR_H_
#define _EASYDB_FIX_SPRITE_COLOR_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <JSON/json.h>

namespace edb
{

class FixSpriteColor : public ICommand
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

private:
	void Trigger(const std::string& dir) const;

	void FixComplex(const wxString& filepath) const;
	void FixAnim(const wxString& filepath) const;

	bool FixSprite(Json::Value& sprite_val) const;

}; // FixSpriteColor 

}

#endif // _EASYDB_FIX_SPRITE_COLOR_H_
