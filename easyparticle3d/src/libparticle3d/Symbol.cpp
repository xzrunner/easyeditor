#include "Symbol.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "FileIO.h"
#include "PSConfigMgr.h"

#include <ps_3d.h>
#include <sprite2/RenderParams.h>
#include <sprite2/P3dEmitterCfg.h>
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
	const s2::P3dEmitterCfg* cfg = GetEmitterCfg();
	if (!cfg) {
		return;
	}
	const p3d_emitter_cfg* impl = cfg->GetImpl();
	for (int i = 0; i < impl->sym_count; ++i)
	{
		const p3d_symbol& p_symbol = impl->syms[i];
		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(p_symbol.ud));
		sym->Traverse(visitor);
	}
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	const s2::P3dEmitterCfg* cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
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