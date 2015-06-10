#ifndef _EASYDB_SEPARATE_TO_COMPLEX_H_
#define _EASYDB_SEPARATE_TO_COMPLEX_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class SeparateToComplex : public ICommand
{
public:
	SeparateToComplex() : m_count(0) {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new SeparateToComplex(); }

private:
	void Run(const std::string& filepath);

private:
	void SeparateSprite(const Json::Value& src, Json::Value& dst);
	void FixSpriteName(const Json::Value& src, Json::Value& dst);

	bool IsSameSprite(const Json::Value& val0, const Json::Value& val1) const;

	std::string SeparateSprite(const Json::Value& value);

	void CreateNewComplexFile(const Json::Value& value, std::string& name, d2d::Vector& pos) const;
	void ResetOldSpriteVal(Json::Value& val, const std::string& name, const d2d::Vector& pos) const;

	void FixPosWithShape(d2d::Vector& pos, const std::string& filepath) const;

private:
	mutable int m_count;

	std::string m_dir;

	std::string m_point_dir;

}; // SeparateToComplex

}

#endif // _EASYDB_SEPARATE_TO_COMPLEX_H_