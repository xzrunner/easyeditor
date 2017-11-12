#ifndef _EASYDB_FIX_TEXT_ALIGN_H_
#define _EASYDB_FIX_TEXT_ALIGN_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixTextAlign : public ICommand, private FixJsonBase
{
public:
	FixTextAlign() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixTextAlign(); }

protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixTextAlign

}

#endif // _EASYDB_FIX_TEXT_ALIGN_H_