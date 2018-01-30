#include "s2loader/P3dSymLoader.h"

#include <logger.h>
#include <sm_const.h>
#include <ps_3d.h>
#include <sprite2/Particle3dSymbol.h>
#include <sprite2/P3dEmitterCfg.h>
#include <simp/NodeParticle3d.h>
#include <bs/FixedPointNum.h>
#include <gum/FilepathHelper.h>
#include <gum/JsonSerializer.h>
#include <gum/SymbolPool.h>

#include <fstream>

#include <string.h>

namespace s2loader
{

P3dSymLoader::P3dSymLoader()
{
}

void P3dSymLoader::Store(const std::shared_ptr<s2::Particle3dSymbol>& sym) const
{
	int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * components.size();
#ifdef USE_MM_ALLOCATOR
	p3d_emitter_cfg* cfg_impl = static_cast<p3d_emitter_cfg*>(mm::AllocHelper::Allocate(sz));
#else
	p3d_emitter_cfg* cfg_impl = reinterpret_cast<p3d_emitter_cfg*>(new uint8_t[sz]);
#endif // USE_MM_ALLOCATOR
	memset(cfg_impl, 0, sz);

	auto cfg = CU_MAKE_SHARED<s2::P3dEmitterCfg>(cfg_impl);
	Store(cfg);

	sym->SetEmitterCfg(cfg);

	sym->SetLoop(loop);
	sym->SetLocal(local);
}

void P3dSymLoader::Store(const std::shared_ptr<s2::P3dEmitterCfg>& p3d_cfg) const
{
	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(p3d_cfg->GetImpl());

	cfg->blend = blend;

	cfg->static_mode = static_mode;

	cfg->emission_time = emission_time;
	cfg->count = count;

	cfg->life = life * 0.001f;
	cfg->life_var = life_var * 0.001f;

	cfg->hori = hori;
	cfg->hori_var = hori_var;
	cfg->vert = vert;
	cfg->vert_var = vert_var;

	cfg->radial_spd = radial_spd;
	cfg->radial_spd_var = radial_spd_var;
	cfg->tangential_spd = tangential_spd;
	cfg->tangential_spd_var = tangential_spd_var;
	cfg->angular_spd = angular_spd;
	cfg->angular_spd_var = angular_spd_var;

	cfg->dis_region = dis_region;
	cfg->dis_region_var = dis_region_var;
	cfg->dis_spd = dis_spd;
	cfg->dis_spd_var = dis_spd_var;

	cfg->gravity = gravity;

	cfg->linear_acc = linear_acc;
	cfg->linear_acc_var = linear_acc_var;

	cfg->fadeout_time = fadeout_time;

	cfg->ground = ground;

	cfg->start_radius = start_radius;
	cfg->start_height = start_height;

	cfg->orient_to_movement = orient_to_movement;

	cfg->blend = blend;

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	cfg->sym_count = components.size();
	cfg->syms = (p3d_symbol*)(cfg+1);
	for (int i = 0, n = components.size(); i < n; ++i) {
		const P3dSymLoader::Component& src = components[i];
		p3d_symbol& dst = cfg->syms[i];

		dst.count = src.count;

		dst.scale_start = src.scale_start * 0.01f;
		dst.scale_end = src.scale_end * 0.01f;

		dst.angle = src.angle * SM_DEG_TO_RAD;
		dst.angle_var = src.angle_var * SM_DEG_TO_RAD;

		memcpy(&dst.mul_col_begin.r, &src.mul_col_begin.r, sizeof(dst.mul_col_begin));
		memcpy(&dst.mul_col_end.r, &src.mul_col_end.r, sizeof(dst.mul_col_end));
		memcpy(&dst.add_col_begin.r, &src.add_col_begin.r, sizeof(dst.add_col_begin));
		memcpy(&dst.add_col_end.r, &src.add_col_end.r, sizeof(dst.add_col_end));

		std::shared_ptr<s2::Symbol> sym = nullptr;
		if (!src.filepath.empty()) {
			sym = LoadSymbol(src.filepath);
		} else {
			sym = gum::SymbolPool::Instance()->Fetch(src.sym_id);
		}
		if (sym) {
			p3d_cfg->InsertCachedSym(sym);
			dst.ud = static_cast<void*>(sym.get());
		} else {
			LOGW("%s", "P3dSymLoader::Store err comp\n");
		}
	}
}

void P3dSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	name = val["name"].asString().c_str();

	loop = val["loop"].asBool();
	local = val["local"].asBool();

	static_mode = val["static_mode"].asBool();

	count = val["count"].asInt();
	emission_time = val["emission_time"].asInt() * 0.001f;

	if (val.isMember("life")) {
		life = static_cast<float>(val["life"]["center"].asInt());
		life_var = static_cast<float>(val["life"]["offset"].asInt());
	} else {
		int min = val["min_life"].asInt(),
			max = val["max_life"].asInt();
		life = (min + max) * 0.5f;
		life_var = (max - min) * 0.5f;
	}

	int min, max;
	min = val["min_hori"].asInt();
	max = val["max_hori"].asInt();
	hori = (min + max) * 0.5f * SM_DEG_TO_RAD;
	hori_var = (max - min) * 0.5f * SM_DEG_TO_RAD;

	min = val["min_vert"].asInt();
	max = val["max_vert"].asInt();
	vert = (min + max) * 0.5f * SM_DEG_TO_RAD;
	vert_var = (max - min) * 0.5f * SM_DEG_TO_RAD;

	if (val.isMember("radial_speed")) {
		radial_spd = static_cast<float>(val["radial_speed"]["center"].asInt());
		radial_spd_var = static_cast<float>(val["radial_speed"]["offset"].asInt());
	} else if (val.isMember("speed")) {
		radial_spd = static_cast<float>(val["speed"]["center"].asInt());
		radial_spd_var = static_cast<float>(val["speed"]["offset"].asInt());
	} else {
		int min, max;
		min = val["min_spd"].asInt();
		max = val["max_spd"].asInt();
		radial_spd = (min + max) * 0.5f;
		radial_spd_var = (max - min) * 0.5f;
	}

	tangential_spd = static_cast<float>(val["tangential_spd"]["center"].asInt());
	tangential_spd_var = static_cast<float>(val["tangential_spd"]["offset"].asInt());

	// todo
	radial_spd *= 0.25f;
	radial_spd_var *= 0.25f;

	if (val.isMember("angular_speed")) {
		angular_spd = val["angular_speed"]["center"].asInt() * SM_DEG_TO_RAD;
		angular_spd_var = val["angular_speed"]["offset"].asInt() * SM_DEG_TO_RAD;
	} else {
		int min, max;
		min = val["min_angular_spd"].asInt();
		max = val["max_angular_spd"].asInt();
		angular_spd = (min + max) * 0.5f * SM_DEG_TO_RAD;
		angular_spd_var = (max - min) * 0.5f * SM_DEG_TO_RAD;
	}

	dis_region = static_cast<float>(val["disturbance_radius"]["center"].asInt());
	dis_region_var = static_cast<float>(val["disturbance_radius"]["offset"].asInt());

	dis_spd = static_cast<float>(val["disturbance_spd"]["center"].asInt());
	dis_spd_var = static_cast<float>(val["disturbance_spd"]["offset"].asInt());

	gravity = static_cast<float>(val["gravity"].asInt());
	// todo
	gravity *= 0.3f;

	linear_acc = static_cast<float>(val["linear_acc"]["center"].asInt());
	linear_acc_var = static_cast<float>(val["linear_acc"]["offset"].asInt());

	inertia = static_cast<float>(val["inertia"].asInt());

	fadeout_time = val["fadeout_time"].asInt() * 0.001f;

	if (!val.isMember("ground")) {
		ground = P3D_GROUND_WITH_BOUNCE;
	} else {
		ground = val["ground"].asInt();
	}

	start_radius = fabs(static_cast<float>(val["start_pos"]["radius"].asInt()));
	start_height = static_cast<float>(val["start_pos"]["height"].asInt());

	orient_to_movement = val["orient_to_movement"].asBool();
	orient_to_parent = val["orient_to_parent"].asBool();

	blend = val["blend"].asInt();

	CU_STR dir = gum::FilepathHelper::Dir(filepath);
	for (int i = 0, n = val["components"].size(); i < n; ++i) {
		LoadComponent(dir, val["components"][i]);
	}
}

void P3dSymLoader::LoadBin(const simp::NodeParticle3d* node)
{
	name.clear();

	static_mode			= bs::int2bool(node->static_mode);

	count				= node->count;
	emission_time		= bs::int2float(node->emission_time, 100);

	life				= bs::int2float(node->life, 100) * 1000;
	life_var			= bs::int2float(node->life_var, 100) * 1000;

	hori				= bs::int2radian((int16_t)(node->hori));
	hori_var			= bs::int2radian(node->hori_var);
	vert				= bs::int2radian((int16_t)(node->vert));
	vert_var			= bs::int2radian(node->vert_var);

	radial_spd			= (int16_t)(node->radial_spd);
	radial_spd_var		= node->radial_spd_var;
	tangential_spd		= (int16_t)(node->tangential_spd);
	tangential_spd_var	= node->tangential_spd_var;
	angular_spd			= bs::int2radian((int16_t)(node->angular_spd));
	angular_spd_var		= bs::int2radian(node->angular_spd_var);

	dis_region			= node->dis_region;
	dis_region_var		= node->dis_region_var;
	dis_spd				= node->dis_spd;
	dis_spd_var			= node->dis_spd_var;

	gravity				= (int16_t)(node->gravity);

	linear_acc			= (int16_t)(node->linear_acc);
	linear_acc_var		= node->linear_acc_var;

	inertia				= 0;

	fadeout_time		= bs::int2float(node->fadeout_time, 100);

	ground				= node->ground;

	start_radius		= node->start_radius;
	start_height		= (int16_t)(node->start_height);

	orient_to_movement	= bs::int2bool(node->orient_to_movement);
	orient_to_parent	= false;

	loop                = bs::int2bool(node->loop);
	local               = bs::int2bool(node->local);

	blend				= node->blend;

	components.clear();
	components.reserve(node->n);
	for (int i = 0; i < node->n; ++i) 
	{
		const simp::NodeParticle3d::Component& src = node->components[i];
		Component dst;
		dst.sym_id			= src.sym_id;
		dst.count			= src.count;
		dst.scale_start		= bs::int2float(src.scale_start, 100) * 100;
		dst.scale_end		= bs::int2float(src.scale_end, 100) * 100;
		dst.angle			= (int16_t)(src.angle);
		dst.angle_var		= src.angle_var;
		dst.mul_col_begin.FromRGBA(src.mul_col_begin);
		dst.mul_col_end.FromRGBA(src.mul_col_end);
		dst.add_col_begin.FromRGBA(src.add_col_begin);
		dst.add_col_end.FromRGBA(src.add_col_end);
		components.push_back(dst);
	}
}

void P3dSymLoader::LoadComponent(const CU_STR& dir, const Json::Value& comp_val)
{
	Component comp;

	if (!comp_val.isMember("count")) {
		comp.count = 0;
	} else {
		comp.count = comp_val["count"].asInt();
	}

	comp.filepath = comp_val["filepath"].asString().c_str();
	comp.filepath = gum::FilepathHelper::Absolute(dir, comp.filepath);

	comp.name = comp_val["name"].asString().c_str();

	if (comp_val.isMember("scale")) {
		comp.scale_start = static_cast<float>(comp_val["scale"]["start"].asInt());
		comp.scale_end = static_cast<float>(comp_val["scale"]["end"].asInt());
	} else {
		comp.scale_start = static_cast<float>(comp_val["start_scale"].asInt());
		comp.scale_end = static_cast<float>(comp_val["end_scale"].asInt());
	}

	if (comp_val.isMember("rotate")) {
		int min = comp_val["rotate"]["min"].asInt(),
			max = comp_val["rotate"]["max"].asInt();
		comp.angle = (min + max) * 0.5f;
		comp.angle_var = (max - min) * 0.5f;
	} else {
		comp.angle = comp.angle_var = 0;
	}

	if (comp_val.isMember("mul_col")) {
		comp.mul_col_begin.r = static_cast<uint8_t>(comp_val["mul_col"]["r"].asDouble() * 255);
		comp.mul_col_begin.g = static_cast<uint8_t>(comp_val["mul_col"]["g"].asDouble() * 255);
		comp.mul_col_begin.b = static_cast<uint8_t>(comp_val["mul_col"]["b"].asDouble() * 255);
		comp.mul_col_begin.a = 255;
		comp.mul_col_end = comp.mul_col_begin;
	} else {
		if (comp_val.isMember("mul_col_begin")) {
			 gum::JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
		} else {
			comp.mul_col_begin.r = comp.mul_col_begin.g = comp.mul_col_begin.b = comp.mul_col_begin.a = 255;
		}
		if (comp_val.isMember("mul_col_end")) {
			 gum::JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
		} else {
			comp.mul_col_end.r = comp.mul_col_end.g = comp.mul_col_end.b = comp.mul_col_end.a = 255;
		}
	}

	if (comp_val.isMember("add_col")) {
		comp.add_col_begin.r = static_cast<uint8_t>(comp_val["add_col"]["r"].asDouble() * 255);
		comp.add_col_begin.g = static_cast<uint8_t>(comp_val["add_col"]["g"].asDouble() * 255);
		comp.add_col_begin.b = static_cast<uint8_t>(comp_val["add_col"]["b"].asDouble() * 255);
		comp.add_col_end = comp.add_col_begin;
	} else {
		if (comp_val.isMember("add_col_begin")) {
			 gum::JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
		} else {
			comp.add_col_begin.r = comp.add_col_begin.g = comp.add_col_begin.b = comp.add_col_begin.a = 0;
		}
		if (comp_val.isMember("add_col_end")) {
			 gum::JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);
		} else {
			comp.add_col_end.r = comp.add_col_end.g = comp.add_col_end.b = comp.add_col_end.a = 0;
		}
	}

	if (comp_val.isMember("alpha")) {
		float start = comp_val["alpha"]["start"].asInt() * 0.01f;
		float end = comp_val["alpha"]["end"].asInt() * 0.01f;
		comp.mul_col_begin.a = static_cast<uint8_t>(comp.mul_col_begin.a * start);
		comp.mul_col_end.a   = static_cast<uint8_t>(comp.mul_col_end.a * end);
		comp.alpha_start = 255 * start;
		comp.alpha_end   = 255 * end;
	}

	if (comp_val.isMember("alpha2")) {
		comp.alpha_start = static_cast<float>(comp_val["alpha2"]["start"].asInt());
		comp.alpha_end   = static_cast<float>(comp_val["alpha2"]["end"].asInt());
	}

	comp.start_z = static_cast<float>(comp_val["start_z"].asInt());

	components.push_back(comp);
}

s2::SymPtr P3dSymLoader::LoadSymbol(const CU_STR& filepath) const
{
	return gum::SymbolPool::Instance()->Fetch(filepath);
}

}