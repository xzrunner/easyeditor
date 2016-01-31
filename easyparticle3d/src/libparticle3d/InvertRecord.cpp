#include "InvertRecord.h"
#include "ParticleSystem.h"

#include <ps_3d.h>

namespace eparticle3d
{

InvertRecord::InvertRecord()
	: m_gravity(0)
{
}

InvertRecord::~InvertRecord()
{
	Clear();
}

void InvertRecord::AddItem(p3d_particle* p)
{
	m_items.push_back(new Item(p));
}

void InvertRecord::StoreToFile(const std::string& filepath) const
{
	Json::Value value;

	value["ps"]["gravity"] = m_gravity;

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i]->StoreToFile(value["sprite"][i]);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, value);
	fout.close();
}

void InvertRecord::Clear()
{
	for_each(m_items.begin(), m_items.end(), DeletePointerFunctor<Item>());
	m_items.clear();
}

//////////////////////////////////////////////////////////////////////////
//	class InvertRecord::Item
//////////////////////////////////////////////////////////////////////////

InvertRecord::Item::
Item(p3d_particle* p)
{
	dead_time = clock();

	life = p->cfg.lifetime;
	for (int i = 0; i < 2; ++i) {
		direction[i] = p->cfg.dir.xy[i];
	}
	for (int i = 0; i < 3; ++i) {
		position[i] = p->pos.xyz[i];
	}
	for (int i = 0; i < 3; ++i) {
		speed[i] = p->spd.xyz[i];
	}	
	linear_acc = p->cfg.linear_acc;
	rotate = p->cfg.angular_spd;
	angle = p->angle;

	symbol = static_cast<ee::Symbol*>(p->cfg.symbol->ud)->GetFilepath();

	start_scale = p->cfg.symbol->scale_start;
	end_scale = p->cfg.symbol->scale_end;

	start_alpha = p->cfg.symbol->alpha_start;
	end_alpha = p->cfg.symbol->alpha_end;

	memcpy(&mul_col.r, &p->cfg.symbol->col_mul.r, sizeof(p->cfg.symbol->col_mul));
	memcpy(&add_col.r, &p->cfg.symbol->col_add.r, sizeof(p->cfg.symbol->col_add));
}

void InvertRecord::Item::
StoreToFile(Json::Value& val) const
{
	val["dead time"] = dead_time;

	val["life"] = life;
	for (int i = 0; i < 2; ++i) {
		val["direction"][i] = direction[i];
	}
	for (int i = 0; i < 3; ++i) {
		val["position"][i] = position[i];
	}
	for (int i = 0; i < 3; ++i) {
		val["speed"][i] = speed[i];
	}
	val["linear_acc"] = linear_acc;
	val["rotate"] = rotate;
	val["angle"] = angle;

	val["symbol"] = symbol;
	val["start_scale"] = start_scale;
	val["end_scale"] = end_scale;
	val["start_alpha"] = start_alpha;
	val["end_alpha"] = end_alpha;

	Json::Value mul_col_val;
	mul_col_val["r"] = mul_col.r;
	mul_col_val["g"] = mul_col.g;
	mul_col_val["b"] = mul_col.b;
	val["mul_col"] = mul_col_val;

	Json::Value add_col_val;
	add_col_val["r"] = add_col.r;
	add_col_val["g"] = add_col.g;
	add_col_val["b"] = add_col.b;
	val["add_col"] = add_col_val;
}

}