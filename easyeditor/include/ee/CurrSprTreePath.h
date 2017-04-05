#ifndef _EASYEDITOR_CURR_SPR_TREE_PATH_H_
#define _EASYEDITOR_CURR_SPR_TREE_PATH_H_

#include <CU_Singleton.h>

#include <stack>

namespace s2 { class Sprite; class Actor; }

namespace ee
{

class CurrSprTreePath
{
public:
	void Push(const s2::Sprite* spr);
	void Pop();

	const s2::Sprite* TopSpr() const;
	const s2::Actor* TopActor() const;

private:
	std::stack<const s2::Sprite*> m_spr_path;
	std::stack<const s2::Actor*> m_actor_path;
	
	SINGLETON_DECLARATION(CurrSprTreePath);

}; // CurrSprTreePath

}

#endif // _EASYEDITOR_CURR_SPR_TREE_PATH_H_