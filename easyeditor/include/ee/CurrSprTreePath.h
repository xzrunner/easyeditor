#ifndef _EASYEDITOR_CURR_SPR_TREE_PATH_H_
#define _EASYEDITOR_CURR_SPR_TREE_PATH_H_

#include <cu/cu_macro.h>
#include <sprite2/typedef.h>

#include <stack>

namespace s2 { class Sprite; class Actor; }

namespace ee
{

class CurrSprTreePath
{
public:
	void Push(const s2::SprPtr& spr);
	void Pop();

	s2::SprPtr TopSpr() const;
	s2::ActorPtr TopActor() const;

private:
	std::stack<s2::SprPtr> m_spr_path;
	std::stack<s2::ActorPtr> m_actor_path;
	
	CU_SINGLETON_DECLARATION(CurrSprTreePath);

}; // CurrSprTreePath

}

#endif // _EASYEDITOR_CURR_SPR_TREE_PATH_H_