#ifndef _EASYDB_SCALE_OVERALL_H_
#define _EASYDB_SCALE_OVERALL_H_

#include "ICommand.h"

#include <SM_Vector.h>

#include <wx/wx.h>
#include <map>

namespace ee { class Snapshoot; }
namespace Json { class Value; }

namespace edb
{

class ScaleOverall : public ICommand
{
public:
	ScaleOverall() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new ScaleOverall(); }

private:
	void Scale(ee::Snapshoot& ss, const std::string& dir, float scale) const;

	void ScaleImage(const std::string& filepath, float scale, ee::Snapshoot& ss,
		std::map<std::string, sm::vec2>& mapImg2Center) const;
	void ScaleComplex(const std::string& filepath, float scale,
		const std::map<std::string, sm::vec2>& mapImg2Center) const;
	void ScaleAnim(const std::string& filepath, float scale,
		const std::map<std::string, sm::vec2>& mapImg2Center) const;

	sm::vec2 GetScaledPos(Json::Value& sprite_val, float scale, 
		const sm::vec2& img_offset) const;

}; // ScaleOverall

}

#endif // _EASYDB_SCALE_OVERALL_H_
