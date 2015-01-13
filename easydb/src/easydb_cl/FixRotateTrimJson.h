#ifndef _EASYDB_FIX_ROTATE_TRIM_JSON_H_
#define _EASYDB_FIX_ROTATE_TRIM_JSON_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <wx/wx.h>
#include <json.h>

namespace edb
{

class FixRotateTrimJson : public ICommand, private FixJsonBase
{
public:
	FixRotateTrimJson() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixRotateTrimJson(); }

protected:
	virtual bool FixSprite(const wxString& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir);

	void LoadTrimInfo(const std::string& dir);

private:
	struct TrimInfo
	{
		float x, y;
		float angle;
	};

private:
	std::string m_dir;

	std::map<std::string, TrimInfo> m_trim_info;

}; // FixRotateTrimJson 

}

#endif // _EASYDB_FIX_ROTATE_TRIM_JSON_H_