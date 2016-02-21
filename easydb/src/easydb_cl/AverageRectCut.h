#ifndef _EASYDB_AVERAGE_RECT_CUT_H_
#define _EASYDB_AVERAGE_RECT_CUT_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class AverageRectCut : public ICommand
{
public:
	AverageRectCut() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new AverageRectCut(); }

private:	
	void Trigger(const std::string& src_dir, const std::string& dst_dir, int min_edge);

private:
	void RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& filepath, int min_edge) const;

	void FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;
	void FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const;

	void FixSpriteValue(const std::string& src_dir, const std::string& dst_dir, 
		const std::string& file_dir, Json::Value& sprite_val) const;

}; // AverageRectCut

}

#endif // _EASYDB_AVERAGE_RECT_CUT_H_