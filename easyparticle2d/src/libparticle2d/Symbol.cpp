#include "Symbol.h"
#include "PSConfigMgr.h"

#include <sprite2/Particle2d.h>
#include <gum/FilepathHelper.h>

namespace eparticle2d
{

Symbol::Symbol()
{
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