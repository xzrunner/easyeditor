#ifndef _EASYDB_RECT_CUT_WITH_JSON_H_
#define _EASYDB_RECT_CUT_WITH_JSON_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class RectCutWithJson : public ICommand
{
public:
	RectCutWithJson() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new RectCutWithJson(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_dir);

private:
	void RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& filepath) const;

	void FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;

	void FixSpriteValue(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& sprite_val) const;

}; // RectCutWithJson

}

#endif // _EASYDB_RECT_CUT_WITH_JSON_H_