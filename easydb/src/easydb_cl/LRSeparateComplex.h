#ifndef _EASYDB_LR_SEPARATE_COMPLEX_H_
#define _EASYDB_LR_SEPARATE_COMPLEX_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class LRSeparateComplex : public ICommand
{
public:
	LRSeparateComplex() : m_count(0) {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	void Run(const std::string& lr_file, const std::string& point_dir,
		const std::string& dst_file);

	static ICommand* Create() { return new LRSeparateComplex(); }

private:
	void Run(const std::string& filepath);

private:
	void SeparateFromSprites(const Json::Value& old_val, Json::Value& new_val);

	void SeparateSprite(const Json::Value& src, Json::Value& dst);
	void FixSpriteName(const Json::Value& src, Json::Value& dst);

	bool IsSameSprite(const Json::Value& val0, const Json::Value& val1) const;

	std::string SeparateSprite(const Json::Value& value);

	std::string CreateNewComplexFile(const Json::Value& value) const;
	void ResetOldSpriteVal(Json::Value& val, const std::string& name, const std::string& tag) const;

	void FixPosWithShape(d2d::Vector& pos, const std::string& filepath) const;

private:
	mutable int m_count;

	std::string m_dir;

	std::string m_point_dir;
	std::string m_output_dir, m_output_name;

}; // LRSeparateComplex

}

#endif // _EASYDB_LR_SEPARATE_COMPLEX_H_