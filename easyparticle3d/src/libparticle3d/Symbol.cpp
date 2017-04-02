#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <sprite2/RenderParams.h>
#include <gum/FilepathHelper.h>

#include <assert.h>

#include <fstream>

namespace eparticle3d
{

Symbol::Symbol()
{	
}

void Symbol::Traverse(ee::Visitor<ee::Sprite>& visitor)
{
	const p3d_emitter_cfg* cfg = GetEmitterCfg();
	if (!cfg) {
		return;
	}
	for (int i = 0; i < cfg->sym_count; ++i)
	{
		const p3d_symbol& p_symbol = cfg->syms[i];
		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(p_symbol.ud));
		sym->Traverse(visitor);
	}
}

sm::rect Symbol::GetBounding(const s2::Sprite* spr, const s2::Actor* actor) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	const p3d_emitter_cfg* cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	SetEmitterCfg(cfg);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	SetLoop(value["loop"].asBool());
	SetLocal(value["local"].asBool());

	return true;
}

}