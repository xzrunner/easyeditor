#include "TrailSymLoader.h"
#include "FilepathHelper.h"
#include "JsonSerializer.h"
#include "Exception.h"
#include "SymbolFactory.h"

#include <mt_2d.h>
#include <sprite2/TrailSymbol.h>

#include <fstream>

#include <assert.h>

namespace glue
{

void TrailSymLoader::Store(s2::TrailSymbol* sym) const
{
	int comp_sz = mode == T2D_MODE_IMAGE ? comp_images.size() : comp_shapes.size();
	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * comp_sz;
	t2d_emitter_cfg* cfg = (t2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);
	Store(cfg);
	sym->SetEmitterCfg(cfg);
}

void TrailSymLoader::Store(t2d_emitter_cfg* cfg) const
{
	cfg->count = count;

	cfg->life_begin = life_begin;
	cfg->life_offset = life_offset;

	cfg->fadeout_time = fadeout_time;

	cfg->mode_type = mode;

	if (mode == T2D_MODE_IMAGE) {
		cfg->sym_count = comp_images.size();
		cfg->syms = (t2d_symbol*)(cfg+1);
		for (int i = 0; i < cfg->sym_count; ++i) {
			const glue::TrailSymLoader::CompImage& src = comp_images[i];
			t2d_symbol& dst = cfg->syms[i];

			memcpy(&dst.col_begin.r, &src.mul_col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.mul_col_end.r, sizeof(dst.col_end));

			memcpy(&dst.mode.A.add_col_begin.r, &src.add_col_begin.r, sizeof(dst.mode.A.add_col_begin));
			memcpy(&dst.mode.A.add_col_end.r, &src.add_col_end.r, sizeof(dst.mode.A.add_col_end));

			dst.mode.A.scale_begin = src.scale_begin;
			dst.mode.A.scale_end = src.scale_end;

			dst.mode.A.ud = LoadSymbol(src.filepath);
			if (!dst.mode.A.ud) {
				throw Exception("TrailSymLoader::Store create sym fail: %s", src.filepath.c_str());
			}
		}
	} else {
		assert(mode == T2D_MODE_SHAPE);
		cfg->sym_count = comp_shapes.size();
		cfg->syms = (t2d_symbol*)(cfg+1);
		for (int i = 0; i < cfg->sym_count; ++i) {
			const glue::TrailSymLoader::CompShape& src = comp_shapes[i];
			t2d_symbol& dst = cfg->syms[i];

			memcpy(&dst.col_begin.r, &src.col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.col_end.r, sizeof(dst.col_end));

			dst.mode.B.size = src.linewidth;
			dst.mode.B.acuity = src.acuity;
		}
	}
}

void TrailSymLoader::LoadJson(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	mode = value["mode"].asInt();

	count = value["count"].asDouble();

	life_begin = value["life_begin"].asDouble() * 0.001f;
	life_offset = value["life_offset"].asDouble() * 0.001f;

	fadeout_time = value["fadeout_time"].asDouble() * 0.001f;

	std::string dir = FilepathHelper::Dir(filepath);
	int idx = 0;
	Json::Value comp_val = value["components"][idx++];
	while (!comp_val.isNull()) {
		if (mode == T2D_MODE_IMAGE) {
			LoadImageComp(dir, comp_val);
		} else {
			assert(mode == T2D_MODE_SHAPE);
			LoadShapeComp(comp_val);
		}
		comp_val = value["components"][idx++];
	}
}

void TrailSymLoader::LoadImageComp(const std::string& dir, const Json::Value& comp_val)
{
	CompImage comp;

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = FilepathHelper::Absolute(dir, comp.filepath);

	comp.scale_begin = comp_val["scale"]["start"].asDouble() * 0.01f;
	comp.scale_end = comp_val["scale"]["end"].asDouble() * 0.01f;

	JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
	JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
	JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
	JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);

	comp_images.push_back(comp);
}

void TrailSymLoader::LoadShapeComp(const Json::Value& comp_val)
{
	CompShape comp;

	comp.linewidth = comp_val["linewidth"].asDouble();
	comp.acuity = comp_val["acuity"].asDouble() * 0.01f;

	JsonSerializer::Load(comp_val["color_begin"], comp.col_begin);
	JsonSerializer::Load(comp_val["color_end"], comp.col_end);
	
	comp_shapes.push_back(comp);
}

s2::Symbol* TrailSymLoader::LoadSymbol(const std::string& filepath) const
{
	return SymbolFactory::Instance()->Create(filepath);
}

}