#include "P2dSymLoader.h"
#include "Exception.h"
#include "FilepathHelper.h"
#include "JsonSerializer.h"
#include "SymbolFactory.h"

#include <sm_const.h>
#include <ps_2d.h>
#include <sprite2/Particle2dSymbol.h>
#include <simp/NodeParticle2d.h>
#include <simp/from_int.h>

#include <fstream>

namespace gum
{

void P2dSymLoader::Store(s2::Particle2dSymbol* sym) const
{
	int sz = SIZEOF_P2D_EMITTER_CFG + SIZEOF_P2D_SYMBOL * components.size();
	p2d_emitter_cfg* cfg = (p2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);
	Store(cfg);
	sym->SetEmitterCfg(cfg);
}

void P2dSymLoader::Store(p2d_emitter_cfg* cfg) const
{
	cfg->mode_type = mode_type;

	if (cfg->mode_type == P2D_MODE_GRAVITY)
	{
		cfg->mode.A.gravity.x = A.gravity.x;
		cfg->mode.A.gravity.y = A.gravity.y;

		cfg->mode.A.speed = A.speed;
		cfg->mode.A.speed_var = A.speed_var;

		cfg->mode.A.tangential_accel = A.tangential_accel;
		cfg->mode.A.tangential_accel_var = A.tangential_accel_var;
		cfg->mode.A.radial_accel = A.radial_accel;
		cfg->mode.A.radial_accel_var = A.radial_accel_var;

		cfg->mode.A.rotation_is_dir = A.rotation_is_dir;
	} 
	else if (cfg->mode_type == P2D_MODE_RADIUS)
	{
		cfg->mode.B.start_radius = B.start_radius;
		cfg->mode.B.start_radius_var = B.start_radius_var;
		cfg->mode.B.end_radius = B.end_radius;
		cfg->mode.B.end_radius_var = B.end_radius_var;

		cfg->mode.B.direction_delta = B.direction_delta;
		cfg->mode.B.direction_delta_var = B.direction_delta_var;
	}
	else if (cfg->mode_type == P2D_MODE_SPD_COS)
	{
		cfg->mode.C.speed = C.speed;
		cfg->mode.C.speed_var = C.speed_var;

		cfg->mode.C.cos_amplitude = C.cos_amplitude;
		cfg->mode.C.cos_amplitude_var = C.cos_amplitude_var;
		cfg->mode.C.cos_frequency = C.cos_frequency;
		cfg->mode.C.cos_frequency_var = C.cos_frequency_var;
	}

	cfg->emission_time = emission_time;
	cfg->count = count;

	cfg->life = life;
	cfg->life_var = life_var;

	cfg->position.x = position.x;
	cfg->position.y = position.y;
	cfg->position_var.x = position_var.x;
	cfg->position_var.y = position_var.y;

	cfg->direction = direction;
	cfg->direction_var = direction_var;

	cfg->sym_count = components.size();
	cfg->syms = (p2d_symbol*)(cfg+1);
	for (int i = 0, n = components.size(); i < n; ++i) 
	{
		const P2dSymLoader::Component& src = components[i];
		p2d_symbol& dst = cfg->syms[i];

		dst.angle_start = src.angle_start * SM_DEG_TO_RAD;
		dst.angle_end = src.angle_end * SM_DEG_TO_RAD;

		dst.scale_start = src.scale_start * 0.01f;
		dst.scale_end = src.scale_end * 0.01f;

		memcpy(&dst.mul_col_begin.r, &src.mul_col_begin.r, sizeof(src.mul_col_begin));
		memcpy(&dst.mul_col_end.r, &src.mul_col_end.r, sizeof(src.mul_col_end));
		memcpy(&dst.add_col_begin.r, &src.add_col_begin.r, sizeof(src.add_col_begin));
		memcpy(&dst.add_col_end.r, &src.add_col_end.r, sizeof(src.add_col_end));

		if (!src.filepath.empty()) {
			dst.ud = LoadSymbol(src.filepath);			
		} else {
			dst.ud = SymbolFactory::Instance()->Create(src.sym_id);
		}
		if (!dst.ud) {
			throw Exception("Symbol doesn't exist: %s", src.filepath.c_str());
		}
	}
}

void P2dSymLoader::LoadJson(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	mode_type = value["mode_type"].asInt();

	if (mode_type == P2D_MODE_GRAVITY) 
	{
		A.gravity.x				= static_cast<float>(value["gravity"]["x"].asDouble());
		A.gravity.y				= static_cast<float>(value["gravity"]["y"].asDouble());

		A.speed					= static_cast<float>(value["speed"]["center"].asDouble());
		A.speed_var				= static_cast<float>(value["speed"]["offset"].asDouble());

		A.tangential_accel		= static_cast<float>(value["tangential_accel"]["center"].asDouble());
		A.tangential_accel_var	= static_cast<float>(value["tangential_accel"]["offset"].asDouble());

		A.radial_accel			= static_cast<float>(value["radial_accel"]["center"].asDouble());
		A.radial_accel_var		= static_cast<float>(value["radial_accel"]["offset"].asDouble());

		A.rotation_is_dir		= value["rotation_is_dir"].asBool();
	}
	else if (mode_type == P2D_MODE_RADIUS) 
	{
		B.start_radius			= static_cast<float>(value["start_radius"]["center"].asDouble());
		B.start_radius_var		= static_cast<float>(value["start_radius"]["offset"].asDouble());

		B.end_radius			= static_cast<float>(value["end_radius"]["center"].asDouble());
		B.end_radius_var		= static_cast<float>(value["end_radius"]["offset"].asDouble());

		B.direction_delta		= static_cast<float>(value["direction_delta"]["center"].asDouble());
		B.direction_delta_var	= static_cast<float>(value["direction_delta"]["offset"].asDouble());		
	}
	else if (mode_type == P2D_MODE_SPD_COS) 
	{
		C.speed					= static_cast<float>(value["speed"]["center"].asDouble());
		C.speed_var				= static_cast<float>(value["speed"]["offset"].asDouble());

		C.cos_amplitude			= static_cast<float>(value["cos_amplitude"]["center"].asDouble());
		C.cos_amplitude_var		= static_cast<float>(value["cos_amplitude"]["offset"].asDouble());

		C.cos_frequency			= static_cast<float>(value["cos_frequency"]["center"].asDouble());
		C.cos_frequency_var		= static_cast<float>(value["cos_frequency"]["offset"].asDouble());
	} 
	else 
	{
		throw Exception("P2dSymLoader::LoadJson unknown mode type.");
	}

	emission_time = static_cast<float>(value["emission_time"].asDouble() * 0.001f);

	count = value["count"].asInt();

	life				= static_cast<float>(value["life"]["center"].asDouble() * 0.001f);
	life_var			= static_cast<float>(value["life"]["offset"].asDouble() * 0.001f);

	position.x			= static_cast<float>(value["position_x"]["center"].asDouble());
	position_var.x		= static_cast<float>(value["position_x"]["offset"].asDouble());
	position.y			= static_cast<float>(value["position_y"]["center"].asDouble());
	position_var.y		= static_cast<float>(value["position_y"]["offset"].asDouble());

	direction			= static_cast<float>(value["direction"]["center"].asDouble() * SM_DEG_TO_RAD);
	direction_var		= static_cast<float>(value["direction"]["offset"].asDouble() * SM_DEG_TO_RAD);

	std::string dir = FilepathHelper::Dir(filepath);
	for (int i = 0, n = value["components"].size(); i < n; ++i) {
		LoadComponent(dir, value["components"][i]);
	}
}

void P2dSymLoader::LoadBin(const simp::NodeParticle2d* node)
{
	name.clear();

	mode_type = node->mode_type;

	if (mode_type == P2D_MODE_GRAVITY)
	{
		A.gravity.x				= node->mode.A.gravity[0];
		A.gravity.y				= node->mode.A.gravity[1];

		A.speed					= node->mode.A.speed;
		A.speed_var				= node->mode.A.speed_var;

		A.tangential_accel		= node->mode.A.tangential_accel;
		A.tangential_accel_var	= node->mode.A.tangential_accel_var;

		A.radial_accel			= node->mode.A.radial_accel;
		A.radial_accel_var		= node->mode.A.radial_accel_var;

		A.rotation_is_dir		= simp::int2bool(node->mode.A.rotation_is_dir);
	}
	else if (mode_type == P2D_MODE_RADIUS) 
	{
		B.start_radius			= node->mode.B.start_radius;
		B.start_radius_var		= node->mode.B.start_radius_var;

		B.end_radius			= node->mode.B.end_radius;
		B.end_radius_var		= node->mode.B.end_radius_var;

		B.direction_delta		= node->mode.B.direction_delta;
		B.direction_delta_var	= node->mode.B.direction_delta_var;		
	}
	else if (mode_type == P2D_MODE_SPD_COS) 
	{
		C.speed					= node->mode.C.speed;
		C.speed_var				= node->mode.C.speed_var;

		C.cos_amplitude			= node->mode.C.cos_amplitude;
		C.cos_amplitude_var		= node->mode.C.cos_amplitude_var;

		C.cos_frequency			= node->mode.C.cos_frequency;
		C.cos_frequency_var		= node->mode.C.cos_frequency_var;
	} 
	else 
	{
		throw Exception("P2dSymLoader::LoadJson unknown mode type.");
	}

	emission_time		= simp::int2float100x(node->emission_time);

	count				= node->count;

	life				= simp::int2float100x(node->life);
	life_var			= simp::int2float100x(node->life_var);

	position.x			= node->position[0];
	position_var.x		= node->position_var[0];
	position.y			= node->position[1];
	position_var.y		= node->position_var[1];

	direction			= simp::int2radian(node->direction);
	direction_var		= simp::int2radian(node->direction_var);

	components.clear();
	components.reserve(node->n);
	for (int i = 0; i < node->n; ++i)
	{
		const simp::NodeParticle2d::Component& src = node->components[i];
		Component dst;
		dst.angle_start		= src.angle_start;
		dst.angle_end		= src.angle_end;
		dst.scale_start		= src.scale_start;
		dst.scale_end		= src.scale_end;
		dst.mul_col_begin.FromRGBA(src.mul_col_begin);
		dst.mul_col_end.FromRGBA(src.mul_col_end);
		dst.add_col_begin.FromRGBA(src.add_col_begin);
		dst.add_col_end.FromRGBA(src.add_col_end);
		dst.sym_id			= src.sym_id;
		components.push_back(dst);
	}
}

void P2dSymLoader::LoadComponent(const std::string& dir, const Json::Value& comp_val)
{
	Component comp;

	comp.angle_start		= comp_val["angle"]["start"].asDouble();
	comp.angle_end			= comp_val["angle"]["end"].asDouble();

	comp.scale_start		= comp_val["scale"]["start"].asDouble();
	comp.scale_end			= comp_val["scale"]["end"].asDouble();

	JsonSerializer::Load(comp_val["mul_col_begin"], comp.mul_col_begin);
	JsonSerializer::Load(comp_val["mul_col_end"], comp.mul_col_end);
	JsonSerializer::Load(comp_val["add_col_begin"], comp.add_col_begin);
	JsonSerializer::Load(comp_val["add_col_end"], comp.add_col_end);

	if (comp_val.isMember("alpha")) {
		comp.alpha_start = comp_val["alpha"]["start"].asInt();
		comp.alpha_end = comp_val["alpha"]["end"].asInt();
	}

	comp.filepath = comp_val["filepath"].asString();
	comp.filepath = FilepathHelper::Absolute(dir, comp.filepath);

	components.push_back(comp);
}

s2::Symbol* P2dSymLoader::LoadSymbol(const std::string& filepath) const
{
	return SymbolFactory::Instance()->Create(filepath);
}

}