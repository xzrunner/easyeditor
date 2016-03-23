#ifndef _EASYDB_SCALE_JSON_H_
#define _EASYDB_SCALE_JSON_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class ScaleJson : public ICommand
{
public:
	ScaleJson() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new ScaleJson(); }

private:
	void Trigger(const std::string& dir, float scale, const std::string& sprite_filename) const;

	void ScaleComplex(const std::string& filepath, float scale, const std::string& sprite_filename) const;
	void ScaleAnim(const std::string& filepath, float scale, const std::string& sprite_filename) const;

	void ScaleSprite(Json::Value& sprite_val, float scale) const;

}; // ScaleJson 

}

#endif // _EASYDB_SCALE_JSON_H_
