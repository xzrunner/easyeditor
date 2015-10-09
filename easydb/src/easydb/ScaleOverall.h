#ifndef _EASYDB_SCALE_OVERALL_H_
#define _EASYDB_SCALE_OVERALL_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <map>

namespace d2d { class Snapshoot; class Vector; }
namespace Json { class Value; }

namespace edb
{

class ScaleOverall : public ICommand
{
public:
	ScaleOverall() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new ScaleOverall(); }

private:
	void Scale(d2d::Snapshoot& ss, const std::string& dir, float scale) const;

	void ScaleImage(const std::string& filepath, float scale, d2d::Snapshoot& ss,
		std::map<std::string, d2d::Vector>& mapImg2Center) const;
	void ScaleComplex(const std::string& filepath, float scale,
		const std::map<std::string, d2d::Vector>& mapImg2Center) const;
	void ScaleAnim(const std::string& filepath, float scale,
		const std::map<std::string, d2d::Vector>& mapImg2Center) const;

	d2d::Vector GetScaledPos(Json::Value& sprite_val, float scale, 
		const d2d::Vector& img_offset) const;

}; // ScaleOverall

}

#endif // _EASYDB_SCALE_OVERALL_H_
