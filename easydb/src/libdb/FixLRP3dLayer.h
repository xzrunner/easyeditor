#ifndef _EASYDB_FIX_LR_P3D_LAYER_H_
#define _EASYDB_FIX_LR_P3D_LAYER_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixLRP3dLayer : public ICommand
{
public:
	FixLRP3dLayer() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new FixLRP3dLayer(); }

protected:
	virtual bool FixSprite(Json::Value& val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixLRP3dLayer 

}

#endif // _EASYDB_FIX_LR_P3D_LAYER_H_