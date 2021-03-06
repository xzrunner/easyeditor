#ifndef _EASYDB_FIX_LR_SPR_LAYER_H_
#define _EASYDB_FIX_LR_SPR_LAYER_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixLRSprLayer : public ICommand
{
public:
	FixLRSprLayer() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixLRSprLayer(); }

protected:
	virtual bool FixSprite(Json::Value& val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixLRSprLayer 

}

#endif // _EASYDB_FIX_LR_SPR_LAYER_H_