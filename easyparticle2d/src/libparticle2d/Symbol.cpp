#include "Symbol.h"
#include "PSConfigMgr.h"

#include <sprite2/Particle2d.h>
#include <gum/FilepathHelper.h>

namespace eparticle2d
{

Symbol::Symbol()
{
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
	m_et_cfg = PSConfigMgr::Instance()->GetConfig(m_filepath);
	return true;
}

}