#ifndef _S2LOADER_SPR_TRANS_LOADER_H_
#define _S2LOADER_SPR_TRANS_LOADER_H_

#include <sprite2/typedef.h>

namespace simp { class NodeTrans; }
namespace sns { class NodeSprCommon; }

namespace s2loader
{

class SprTransLoader
{
public:
	static void Load(const s2::SprPtr& spr, const simp::NodeTrans* trans);
	static void Load(const s2::SprPtr& spr, const sns::NodeSprCommon& common);
	
private:
	static float ToFloat(int i, int precision = 1024);

}; // SprTransLoader

}

#endif // _S2LOADER_SPR_TRANS_LOADER_H_
