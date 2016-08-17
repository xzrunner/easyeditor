#include "InvertRecord.h"
#include "ParticleSystem.h"

#include <ee/std_functor.h>
#include <ee/Symbol.h>
#include <ee/JsonSerializer.h>

#include <ps_3d.h>

#include <fstream>
#include <algorithm>

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
	for_each(m_items.begin(), m_items.end(), ee::DeletePointerFunctor<Item>());
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

	sym = static_cast<ee::Symbol*>(p->cfg.sym->ud)->GetFilepath();

	scale_start = p->cfg.sym->scale_start;
	scale_end = p->cfg.sym->scale_end;

	memcpy(&mul_col_begin.r, &p->cfg.sym->mul_col_begin.r, sizeof(p->cfg.sym->mul_col_begin));
	memcpy(&mul_col_end.r, &p->cfg.sym->mul_col_end.r, sizeof(p->cfg.sym->mul_col_end));
	memcpy(&add_col_begin.r, &p->cfg.sym->add_col_begin.r, sizeof(p->cfg.sym->add_col_begin));
	memcpy(&add_col_end.r, &p->cfg.sym->add_col_end.r, sizeof(p->cfg.sym->add_col_end));
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

	val["symbol"] = sym;
	val["start_scale"] = scale_start;
	val["end_scale"] = scale_end;

	ee::JsonSerializer::Store(mul_col_begin, val["mul_col_begin"]);
	ee::JsonSerializer::Store(mul_col_end, val["mul_col_end"]);
	ee::JsonSerializer::Store(add_col_begin, val["add_col_begin"]);
	ee::JsonSerializer::Store(add_col_end, val["add_col_end"]);
}

}