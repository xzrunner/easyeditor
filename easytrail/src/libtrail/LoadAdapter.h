#ifndef _EASYTRAIL_LOAD_ADAPTER_H_
#define _EASYTRAIL_LOAD_ADAPTER_H_

#include <sprite2/Color.h>

#include <json/json.h>

#include <string>

namespace etrail
{

class LoadAdapter
{
public:
	void Load(const std::string& filepath);

private:
	void LoadImageComp(const std::string& dir, const Json::Value& comp_val);
	void LoadShapeComp(const Json::Value& comp_val);

public:
	struct CompImage
	{
		std::string filepath;
		float scale_begin, scale_end;
		s2::Color mul_col_begin, mul_col_end;
		s2::Color add_col_begin, add_col_end;
	}; // CompImage

	struct CompShape
	{
		float linewidth;
		float acuity;
		s2::Color col_begin, col_end;
	}; // CompShape

public:
	int mode;

	int count;

	float life_begin;
	float life_offset;

	float fadeout_time;

	std::vector<CompImage> comp_images;
	std::vector<CompShape> comp_shapes;

}; // LoadAdapter

}

#endif // _EASYTRAIL_LOAD_ADAPTER_H_