#ifndef _EASYDB_LR_EXPAND_GROUP_H_
#define _EASYDB_LR_EXPAND_GROUP_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <json/json.h>

namespace edb
{

class LRExpandGroup : public ICommand
{
public:
	LRExpandGroup() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRExpandGroup(); }

	void Run(const std::string& filepath);

	std::string GetOutputFilepath(const std::string& filepath) const;

private:
	struct Trans
	{
		Trans() : translation(0, 0), angle(0), scale(1, 1), xmirror(false), ymirror(false) {}

		sm::vec2 translation;
		float angle;
		sm::vec2 scale;
		bool xmirror, ymirror;
	};

private:
	void Expand(const Json::Value& src_val, Json::Value& dst_val);

	void LoadSprites(const Json::Value& src_spr_val, const Trans& trans, Json::Value& dst_sprs_val);

}; // LRExpandGroup

}

#endif // _EASYDB_LR_EXPAND_GROUP_H_