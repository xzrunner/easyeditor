#include "s2loader/TrailSymLoader.h"

#include <logger.h>
#include <mt_2d.h>
#include <sprite2/TrailSymbol.h>
#include <sprite2/TrailEmitterCfg.h>
#include <simp/NodeTrail.h>
#include <bs/FixedPointNum.h>
#include <gum/FilepathHelper.h>
#include <gum/JsonSerializer.h>
#include <gum/SymbolPool.h>

#include <fstream>

#include <assert.h>
#include <string.h>

namespace s2loader
{

TrailSymLoader::TrailSymLoader()
{
}

void TrailSymLoader::Store(const std::shared_ptr<s2::TrailSymbol>& sym) const
{
	int comp_sz = mode == T2D_MODE_IMAGE ? comp_images.size() : comp_shapes.size();
	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * comp_sz;
	t2d_emitter_cfg* cfg_impl = (t2d_emitter_cfg*) operator new(sz);
	memset(cfg_impl, 0, sz);
	Store(cfg_impl);

	auto cfg = std::make_shared<s2::TrailEmitterCfg>(cfg_impl);
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
			const TrailSymLoader::CompImage& src = comp_images[i];
			t2d_symbol& dst = cfg->syms[i];

			memcpy(&dst.col_begin.r, &src.mul_col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.mul_col_end.r, sizeof(dst.col_end));

			memcpy(&dst.mode.A.add_col_begin.r, &src.add_col_begin.r, sizeof(dst.mode.A.add_col_begin));
			memcpy(&dst.mode.A.add_col_end.r, &src.add_col_end.r, sizeof(dst.mode.A.add_col_end));

			dst.mode.A.scale_begin = src.scale_begin;
			dst.mode.A.scale_end = src.scale_end;

			std::shared_ptr<s2::Symbol> sym = nullptr;
			if (!src.filepath.empty()) {
				sym = LoadSymbol(src.filepath);
			} else {
				sym = gum::SymbolPool::Instance()->Fetch(src.sym_id);
			}
			if (sym) {
				// todo: cache sym
//				cfg->InsertCachedSym(sym);
				dst.mode.A.ud = static_cast<void*>(sym.get());
			} else {
				LOGW("%s", "TrailSymLoader::Store err comp\n");
			}
		}
	} else {
		assert(mode == T2D_MODE_SHAPE);
		cfg->sym_count = comp_shapes.size();
		cfg->syms = (t2d_symbol*)(cfg+1);
		for (int i = 0; i < cfg->sym_count; ++i) {
			const TrailSymLoader::CompShape& src = comp_shapes[i];
			t2d_symbol& dst = cfg->syms[i];

			memcpy(&dst.col_begin.r, &src.col_begin.r, sizeof(dst.col_begin));
			memcpy(&dst.col_end.r, &src.col_end.r, sizeof(dst.col_end));

			dst.mode.B.size = src.linewidth;
			dst.mode.B.acuity = src.acuity;
		}
	}
}

void TrailSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	mode = value["mode"].asInt();

	count = static_cast<int>(value["count"].asDouble());

	life_begin = static_cast<float>(value["life_begin"].asDouble() * 0.001f);
	life_offset = static_cast<float>(value["life_offset"].asDouble() * 0.001f);

	fadeout_time = static_cast<float>(value["fadeout_time"].asDouble() * 0.001f);

	CU_STR dir = gum::FilepathHelper::Dir(filepath);
	for (int i = 0, n = value["components"].size(); i < n; ++i)
	{
		if (mode == T2D_MODE_IMAGE) {
			LoadImageComp(dir, value["components"][i]);
		} else {
			assert(mode == T2D_MODE_SHAPE);
			LoadShapeComp(value["components"][i]);
		}
	}
}

void TrailSymLoader::LoadBin(const simp::NodeTrail* node)
{
	mode			= node->mode_type;

	count			= node->count;

	life_begin		= bs::int2float(node->life_begin, 100);
	life_offset		= bs::int2float(node->life_offset, 100);

	fadeout_time	= bs::int2float(node->fadeout_time, 100);

	int n = node->n;
	if (mode == T2D_MODE_IMAGE)
	{
		comp_images.reserve(n);
		for (int i = 0; i < n; ++i)
		{
			const simp::NodeTrail::Component& src = node->components[i];
			CompImage dst;
			dst.sym_id      = src.mode.A.sym_id;
			dst.scale_begin	= bs::int2float(src.mode.A.scale_begin, 100);
			dst.scale_end   = bs::int2float(src.mode.A.scale_end, 100);
			dst.mul_col_begin.FromRGBA(src.col_begin);
			dst.mul_col_end.FromRGBA(src.col_end);
			dst.add_col_begin.FromRGBA(src.mode.A.add_col_begin);
			dst.add_col_end.FromRGBA(src.mode.A.add_col_end);
			comp_images.push_back(dst);
		}
	}
	else
	{
		assert(mode == T2D_MODE_SHAPE);
		comp_shapes.reserve(n);
		for (int i = 0; i < n; ++i)
		{
			const simp::NodeTrail::Component& src = node->components[i];
			CompShape dst;
			dst.linewidth = bs::int2float(src.mode.B.size, 100);
			dst.acuity	  = bs::int2float(src.mode.B.acuity, 100);
			dst.col_begin.FromRGBA(src.col_begin);
			dst.col_end.FromRGBA(src.col_end);
			comp_shapes.push_back(dst);
		}
	}
}

void TrailSymLoader::LoadImageComp(const CU_STR& dir, const Json::Value& comp_val)
{
	CompImage comp;

	comp.filepath = comp_val["filepath"].asString().c_str();
	comp.filepath = gum::FilepathHelper::Absolute(dir, comp.filepath);

	comp.scale_begin = static_cast<float>(comp_val["scale"]["start"].asDouble() * 0.01f);
	comp.scale_end = static_cast<float>(comp_val["scale"]["end"].asDouble() * 0.01f);

	 gum::JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
	 gum::JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
	 gum::JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
	 gum::JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);

	comp_images.push_back(comp);
}

void TrailSymLoader::LoadShapeComp(const Json::Value& comp_val)
{
	CompShape comp;

	comp.linewidth = static_cast<float>(comp_val["linewidth"].asDouble());
	comp.acuity = static_cast<float>(comp_val["acuity"].asDouble() * 0.01f);

	 gum::JsonSerializer::Load(comp_val["color_begin"], comp.col_begin);
	 gum::JsonSerializer::Load(comp_val["color_end"], comp.col_end);
	
	comp_shapes.push_back(comp);
}

s2::SymPtr TrailSymLoader::LoadSymbol(const CU_STR& filepath) const
{
	return gum::SymbolPool::Instance()->Fetch(filepath);
}

}