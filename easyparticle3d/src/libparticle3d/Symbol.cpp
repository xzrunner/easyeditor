#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <sprite2/RenderParams.h>

#include <assert.h>

#include <fstream>

namespace eparticle3d
{

Symbol::Symbol()
{	
}

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	if (!m_et_cfg) {
		return;
	}
	for (int i = 0; i < m_et_cfg->sym_count; ++i)
	{
		const p3d_symbol& p_symbol = m_et_cfg->syms[i];
		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(p_symbol.ud));
		sym->Traverse(visitor);
	}
}

void Symbol::LoadResources()
{
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = p3d_emitter_create(m_et_cfg);
	p3d_emitter_start(m_et);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	m_loop = value["loop"].asBool();
	m_local = value["local"].asBool();
}

}