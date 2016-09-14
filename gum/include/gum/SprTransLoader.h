#ifndef _GUM_SPR_TRANS_LOADER_H_
#define _GUM_SPR_TRANS_LOADER_H_

namespace s2 { class Sprite; }
namespace simp { class NodeTrans; }

namespace gum
{

class SprTransLoader
{
public:
	static void Load(s2::Sprite* spr, const simp::NodeTrans* trans);

private:
	static float ToFloat(int i);

}; // SprTransLoader

}

#endif // _GUM_SPR_TRANS_LOADER_H_